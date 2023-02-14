<script lang="ts">
	import { onMount } from 'svelte';
	import { each } from 'svelte/internal';
	import IndividualMotor from './IndividualMotor.svelte';
	import TrackSwitchers from './TrackSwitchers.svelte';
	import Paho from 'paho-mqtt';
	let client: Paho.Client;
	let connected = false;
	onMount(() => {
		//Connect to MQTT server
		connect();
	});
	function connect() {
		//192.168.0.52 9001
		client = new Paho.Client('localhost', 9001, '');
		client.onConnectionLost = onConnectionLost;
		client.onMessageArrived = onMessageArrived;
		client.connect({ onSuccess: onConnect, onFailure: connect });
	}
	function onConnect() {
		connected = true;
	}
	function onConnectionLost() {
		connected = false;
		connect();
	}
	function onMessageArrived() {}

	function setSpeed(id: number, val: number) {
		//Template for setting the motor speed value MQTT command
		let temp = '{"Id":' + id + ',"Value":' + val + '}';
		let message = new Paho.Message(temp);
		message.destinationName = 'motor/cmd/speed';
		client?.send(message);
	}
	function handleSpeed(event: CustomEvent<any>){
		//Listen to the setSpeed event dispatched from the indiviual motors
		setSpeed(event.detail.id, event.detail.value);
	}
	function handleSwitch(event: CustomEvent<any>){
		
	}
	let RailwayTrack = [0,0];
	let current: boolean = false;

	let Switches: boolean[] = [
		true,
		true,
		true,
		true,
		true,
		true
	]
</script>

<div>
	{#if connected}
	<div class="center">
		<button on:click={() => current = !current}>◀️</button>
		<button on:click={() => current = !current}>▶️</button>
	</div>
	{#if current}
	<div class="TrainControl center">
		{#each RailwayTrack as Track, i}
		<IndividualMotor bind:Value={Track} Id={i} on:setSpeed={handleSpeed}/>
		{/each}
	</div>
	{:else}
	<div class="center">
		<TrackSwitchers bind:Switches={Switches} on:setSwitch={handleSwitch}/>
	</div>
	{/if}
    {:else}
        <h2 class="center">Venter på forbinnelse med server...</h2>
    {/if}
</div>

<style>
	button{
		font-size: xx-large;
		border: none;
		background: none;
		text-shadow: 1px 1px 2px black;
		cursor: pointer;
	}
	button:active{
		transform: translateY(3px);
		text-shadow: none;
	}
	h2{
		font-size: large;
        font-family: 'Courier New', Courier, monospace;
		margin-top: 5em;
	}
    .TrainControl{
		padding: 0.25em;
        background-color: beige;
        border-radius: 3em;
		width: min-content;
    }
	.center{
		margin: auto;
		text-align: center;
	}
</style>