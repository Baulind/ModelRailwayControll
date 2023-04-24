<script lang="ts">
	import { onMount } from 'svelte';
	import TrackSwitchers from './TrackSwitchers.svelte';
	import { Broker, railwayTrack, switches, indicator, uuid } from '../stores'
	import Paho, { Message } from 'paho-mqtt';
	import MotorController from './MotorController.svelte';

	let client: Paho.Client;
	let current: boolean = false;
	let connected: boolean = false;
	let users: number = 1;

	onMount(() => {
		//Connect to MQTT server
		connect();
	});

	function connect() {
		client = new Paho.Client($Broker, 9001, $uuid);
		client.onConnectionLost = onConnectionLost;
		client.onMessageArrived = onMessageArrived;
		client.connect({ onSuccess: onConnect, onFailure: connect });
	}

	function onConnect() {
		connected = true;
		client.subscribe('$SYS/broker/clients/active');
		client.subscribe('motor/cmd/speed');
		client.subscribe('switch/cmd/state');
		client.subscribe('Sensors/Reading');
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
			$railwayTrack[temp.Id] = temp.Speed
		} 
		else if (message.destinationName === 'switch/cmd/state'){
			let temp: {Id: number, State: boolean};
			temp = JSON.parse(message.payloadString);
			$switches[temp.Id] = temp.State
		}
		else if(message.destinationName === 'Sensors/Reading'){
			let temp: {Id: number, Value: number};
			temp = JSON.parse(message.payloadString);
			console.log(temp);
			$indicator[temp.Id] = temp.Value === 1;
		}
	}

	function setSpeed(Id: number, Speed: number, Sender: string) {
		//Template for setting the motor speed value MQTT command
		let message = new Paho.Message(JSON.stringify({Id, Speed, Sender}));
		message.destinationName = 'motor/cmd/speed';
		client?.send(message);
	}
	function setSwitch(Id: number, State: boolean, Sender: string){
		let message = new Paho.Message(JSON.stringify({Id, State, Sender}));
		message.destinationName = 'switch/cmd/state';
		client?.send(message);
	}
	function handleSpeed(event: CustomEvent<any>){
		setSpeed(event.detail.id, event.detail.value, $uuid);
	}
	function handleSwitch(event: CustomEvent<any>){
		setSwitch(event.detail.id, event.detail.value, $uuid);
	}

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
	<MotorController on:setSpeed={handleSpeed}/>
	{:else}
	<TrackSwitchers on:setSwitch={handleSwitch}/>
	{/if}
	<br>
	<div class="center card">
		<h2>{users} Active:</h2>
		{#each [...Array(users).keys()] as n}
			<span>🎅</span>
		{/each}
		<p>{$uuid}</p>
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