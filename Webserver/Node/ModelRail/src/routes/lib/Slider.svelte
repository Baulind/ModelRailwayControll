<script lang="ts">
    import type { Motor } from "src/types/Motor.type";
    export let motor: Motor;
    $: console.log(motor.val)
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