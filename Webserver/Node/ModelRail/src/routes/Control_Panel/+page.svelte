<script lang="ts">
	import { onMount } from 'svelte';
	import TrackSwitchers from './TrackSwitchers.svelte';
	import Paho, { Message } from 'paho-mqtt';
	import MotorController from './MotorController.svelte';
	let client: Paho.Client;
	let connected: boolean = false;
	let users: number = 1;
	onMount(() => {
		//Connect to MQTT server
		connect();
	});
	function connect() {
		//192.168.0.52 9001
		//client = new Paho.Client('192.168.0.219', 9001, '');
		//client = new Paho.Client('192.168.0.114', 9001, '');
		client = new Paho.Client('localhost', 9001, '');
		client.onConnectionLost = onConnectionLost;
		client.onMessageArrived = onMessageArrived;
		client.connect({ onSuccess: onConnect, onFailure: connect });
	}
	function onConnect() {
		connected = true;
		client.subscribe('$SYS/broker/clients/active');
		client.subscribe('motor/cmd/speed');
		client.subscribe('switch/cmd/state');
	}
	function onConnectionLost() {
		connected = false;
		connect();
	}
	function onMessageArrived(message: Message) {
		if(message.destinationName === '$SYS/broker/clients/active'){
			users = +message.payloadString
		}
		else if (message.destinationName === 'motor/cmd/speed') {
			let temp: {Id: number, Speed: number};
			temp = JSON.parse(message.payloadString);
			RailwayTrack[temp.Id] = temp.Speed
		} 
		else if (message.destinationName === 'switch/cmd/state'){
			let temp: {Id: number, State: boolean};
			temp = JSON.parse(message.payloadString);
			Switches[temp.Id] = temp.State
		}
	}

	function setSpeed(Id: number, Speed: number) {
		//Template for setting the motor speed value MQTT command
		let message = new Paho.Message(JSON.stringify({Id, Speed}));
		message.destinationName = 'motor/cmd/speed';
		client?.send(message);
	}
	function setSwitch(Id: number, State: boolean){
		let message = new Paho.Message(JSON.stringify({Id, State}));
		message.destinationName = 'switch/cmd/state';
		client?.send(message);
	}
	function handleSpeed(event: CustomEvent<any>){
		setSpeed(event.detail.id, event.detail.value);
	}
	function handleSwitch(event: CustomEvent<any>){
		setSwitch(event.detail.id, event.detail.value);
	}

	//Values to bind for debug and dev purpouses 
	let RailwayTrack: number[] = [0, 0];
	let Switches: boolean[] = [true, true, true, true, true, true]
	let current: boolean = true;
</script>

{#if !connected}
	<h2 class="center">Venter på forbinnelse med server...</h2>
{:else}
	<div class="center">
		<button on:click={() => current = !current}>◀️</button>
		<button on:click={() => current = !current}>▶️</button>
	</div>
	<br>
	{#if current}
	<MotorController values={RailwayTrack} on:setSpeed={handleSpeed}/>
	{:else}
	<TrackSwitchers bind:Switches={Switches} on:setSwitch={handleSwitch}/>
	{/if}
	<br>
	<div class="center card">
		<h2>{users} Active:</h2>
		{#each [...Array(users).keys()] as n}
			<span>🎅</span>
		{/each}
	</div>
{/if}


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
	}
	.center{
		margin: auto;
		text-align: center;
	}
	.card{
		padding: 0 1em 1em 1em;
		border-radius: 1.5em;
		border: solid 0.5em beige;
		box-sizing: border-box;
		width: fit-content;
		max-width: 35em;
	}
</style>