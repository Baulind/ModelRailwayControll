<script lang="ts">
    import { onMount } from "svelte";
    import type { Motor } from "src/types/Motor.type";
    import Paho from 'paho-mqtt';
    export let motor: Motor;
    let client: any;
    onMount(() => {
        //ws://192.168.0.52
        client = new Paho.Client("192.168.0.52", 1883, "browser", "");
    });
    $: {
        let message = new Paho.Message('{\"Id\":0,\"Command\":\"setSpeed\",\"Args\":[{' + motor.val + '}]}');
        //message.destinationName = "motor/cmd";
        client.send(message);
    }
</script>

<div class="slidecontainer">
    <input type="range" min={motor.min.toString()} max={motor.max.toString()} bind:value={motor.val} class="slider">
</div>

<style>
    .slider {
    -webkit-appearance: none;
    width: 100%;
    height: 15px;
    border-radius: 5px;  
    background: #d3d3d3;
    outline: none;
    opacity: 0.7;
    -webkit-transition: .2s;
    transition: opacity .2s;
    }

    .slider::-webkit-slider-thumb {
    -webkit-appearance: none;
    appearance: none;
    width: 25px;
    height: 25px;
    border-radius: 50%; 
    background: #04AA6D;
    cursor: pointer;
    }

    .slider::-moz-range-thumb {
    width: 25px;
    height: 25px;
    border-radius: 50%;
    background: #04AA6D;
    cursor: pointer;
    }
    .slidecontainer{
        margin: auto 0;
    }
</style>