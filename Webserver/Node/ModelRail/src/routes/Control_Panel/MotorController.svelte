<script lang="ts">
	import IndividualMotor from "./IndividualMotor.svelte";    
    import { createEventDispatcher } from 'svelte';
	import { each } from "svelte/internal";
    export let values: number[];
    const dispatch = createEventDispatcher();
    function handleSpeed(event: CustomEvent<any>){
        dispatch('setSpeed', {
			id: event.detail.id,
			value: event.detail.value
		});
    }
    let selected = 0;
    
    function onClick(i: number){
        //Get all tabs and content
        let tabs = document.getElementById("tabs");
        let container = document.getElementById("container");
        //set current tab to unfocused collor
        let tab = tabs?.children[selected] as HTMLElement;
        tab.style.backgroundColor = "gray";
        let content = container?.children[selected] as HTMLElement;
        content.style.display = "none";

        selected = i;
        
        //Now that selected is new set focus collor
        tab = tabs?.children[selected] as HTMLElement;
        tab.style.backgroundColor = "beige"
        content = container?.children[selected] as HTMLElement;
        content.style.display = "block"
    }
</script>

{#if values.length >= 1}
    <div class="body center">
        <div class="tabBar" id="tabs">
        {#if values.length != 1}
            {#each values as v, i}
                <button class="tab" on:click={() => onClick(i)}>Tog: {i}</button>
            {/each}
        {:else}
            <div class="tab"></div>
        {/if}
        </div>
        <div class="container" id="container">
            {#each values as v, i}
            <div class="controller">
                <h2>Motor: {i} Value: {v}</h2>
                <IndividualMotor bind:Value={v} Id={i} on:setSpeed={handleSpeed}/>
            </div>
            {/each}
        </div>
    </div>
{:else}
    <h2>No registered trains</h2>
{/if}

<style>
    .body{
        max-width: 35em;
        min-width: min-content;
        margin: auto;
        text-align: center;
    }
    .tabBar{
        display: flex;
        justify-content: space-around;
        min-width: fit-content;
    }
    .tabBar > :first-child{
        background-color: beige;
    }
    .tab{
        padding: 1em;
        background-color: gray;
        width: 100%;
        border-radius: 1.5em 1.5em 0 0;
    }
    .container{
        padding: 2em;
        background-color: beige;
        border-radius: 0 0 1.5em 1.5em;
    }
    .controller{
        display: none;
    }
    .container > :first-child{
        display: block;
    }
    button{
        border: none;
        font-family: 'Courier New', Courier, monospace;
        font-size: 1em;
        font-weight: bold;
    }
    h2{
        font-family: 'Courier New', Courier, monospace;
        font-weight: bold;
        text-align: center;
    }
</style>