<script lang="ts">
	import { each } from "svelte/internal";
	import Pin from './Pin.svelte';
    let amount: number = 1;
    const max: number = 12;
    const min: number = 1;
    
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
    <div class="configureMotor shape">
        <h2>Set amount of motors.</h2>
        <div>
            <span>Number:</span>
            <span><input type="number" min="{min}" max="{max}" bind:value={amount}></span>
        </div>
        <button class="shape submit" on:click={Generate}>Generate configuration.</button>
        {#if pins.length > 0}
            <button class="shape submit" on:click={() => {}}>Send</button>
        {/if}
    </div>
    <div class="center">
        {#if pins.length > 0}
            <h3>Verify pin-numbers: </h3>
            <div class="scrollable shape">
                {#each pins || [] as pin, i}
                    <Pin bind:Output={pin.Output} bind:Direction={pin.Direction} Id={i}></Pin>
                {/each}
            </div>
        {/if}
    </div>
</div>


<style>
    .center{
        margin: auto;
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
