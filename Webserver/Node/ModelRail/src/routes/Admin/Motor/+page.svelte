<script lang="ts">
	import { each } from "svelte/internal";
	import Pin from './Pin.svelte';
    let amount: number = 0;
    const max: number = 12;
    const min: number = 0;
    
	import { onMount } from "svelte";
    import Paho from 'paho-mqtt';
	let client: Paho.Client;
	let connected = false;
	onMount(() => {
		//Connect to MQTT server
		connect();
	});
	function connect() {
		//192.168.0.52 9001
		//client = new Paho.Client('192.168.0.219', 9001, '');
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

    function setConfig(){
        //Template for setting the motor speed value MQTT command
        const Command: string = "setPins";
        let Value: number[] = [];
        for (let Id = 0; Id < pins.length; Id++) {
            Value = [pins[Id].Output, pins[Id].Direction]
            let message = new Paho.Message(JSON.stringify({Id, Command, Value}));
            message.destinationName = 'motor/cmd';
            client?.send(message);
        }
    }

    function Generate(){
        pins = [];
        for (let i = 1; i < amount*2; i += 2) {
            const obj = {Output: i, Direction: i+1}
            pins.push(obj);
        }
    }

    let pins: {
        Output: number;
        Direction: number;
    }[] = [];
    
    $:{
        if (amount > max) {
            amount = max;
        } 
        else if (amount < min) {
            amount = min;
        }
    }
</script>

<div class="page">
    {#if !connected}
        <h2 class="center">Venter på forbinnelse med server...</h2>
    {:else}
    <div class="configureMotor shape">
        <h2>Set amount of motors.</h2>
        <div>
            <span>Number:</span>
            <span><input type="number" min={min} max={max} bind:value={amount}></span>
        </div>
        <button class="shape submit" on:click={Generate}>Generate configuration.</button>
        {#if pins.length > 0}
            <button class="shape submit" on:click={setConfig}>Send</button>
        {/if}
    </div>
    <div class="center">
        {#if pins.length > 0}
            <h3>Verify pin-numbers: </h3>
            <div class="scrollable shape">
                {#each pins || [] as pin, i}
                    <Pin bind:Output={pin.Output} bind:Direction={pin.Direction} Id={i}/>
                {/each}
            </div>
        {/if}
    </div>
    {/if}
</div>


<style>
    .center{
        margin: auto;
        text-align: center;
    }
    .scrollable{
        overflow-y: scroll;
        max-height: 17.5em;
        max-width: max-content;
        margin: auto;
    }
    h3{
        text-align: center;
    }
    .page{
        font-family: 'Courier New', Courier, monospace;
    }
    button{
        font-size: large;
        font-family: 'Courier New', Courier, monospace;
        padding: 1em;
        border: none;
        background-color: white;
        box-shadow: 1px 1px 2px black;
    }
    button:active{
        transform: translateY(2px);
        box-shadow: none;
    }
    .shape{
        border-radius: 3em;
    }
    .configureMotor{
        padding: 1.5em;
        margin: auto;
        display: flex;
        flex-direction: column;
        background-color: beige;
        gap: 1em;
        width: max-content;
        height: max-content;
    }
</style>
