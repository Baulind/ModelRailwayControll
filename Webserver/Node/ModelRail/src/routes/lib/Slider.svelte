<script lang="ts">
	import { onMount } from 'svelte';
	import type { Motor } from 'src/types/Motor.type';
	import Paho from 'paho-mqtt';
	export let motor: Motor;
	let client: Paho.Client;
	onMount(() => {
		//ws://192.168.0.52
		client = new Paho.Client('192.168.0.52', 9001, '', 'browser');
		client.onConnectionLost = onConnectionLost;
		client.onMessageArrived = onMessageArrived;
		client.connect({ onSuccess: onConnect });
	});

	function onConnect() {}
	function onConnectionLost() {}
	function onMessageArrived() {}

	function onDrag() {
		let temp = '{"Id":0,"Command":"setSpeed","Args":[' + motor.val + ']}';
		let message = new Paho.Message(temp);
		message.destinationName = 'motor/cmd';
		client.send(message);
	}
</script>

<div class="slidecontainer">
	<input
		type="range"
		min={motor.min.toString()}
		max={motor.max.toString()}
		bind:value={motor.val}
		on:change={onDrag}
		class="slider"
	/>
</div>

<style>
	.slider {
		width: 100%;
		height: 15px;
		border-radius: 5px;
		background: #d3d3d3;
		outline: none;
		opacity: 0.7;
		-webkit-transition: 0.2s;
		transition: opacity 0.2s;
	}

	.slider::-webkit-slider-thumb {
		-webkit-appearance: none;
		appearance: none;
		width: 25px;
		height: 25px;
		border-radius: 50%;
		background: #04aa6d;
		cursor: pointer;
	}

	.slider::-moz-range-thumb {
		width: 25px;
		height: 25px;
		border-radius: 50%;
		background: #04aa6d;
		cursor: pointer;
	}
	.slidecontainer {
		margin: auto 0;
	}
</style>
