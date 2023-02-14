<script lang="ts">
	import IndividualSwitch from "./IndividualSwitch.svelte";
    import { createEventDispatcher } from 'svelte';
    export let Switches: boolean[] = [];
    
	const dispatch = createEventDispatcher();
    function handleSwitch(event: CustomEvent<any>){
        dispatch('setSwitch', {
			id: event.detail.id,
			value: event.detail.value
		});
    }
    
	const cords = [
		{x: 5, y: 30}, 
		{x: 15, y: 40},
		{x: 15, y: 70},
		{x: 15, y: 75},
		{x: 20, y: 88},
		{x:77.5, y: 70}
	]
    
</script>

<div class="TrackSwitcher">
    {#each Switches || [] as Switch, i}
    <div class="container" style="left: {cords[i].x}%; top: {cords[i].y}%;">
        <IndividualSwitch Id={i} bind:Value={Switch} on:setSwitch={handleSwitch}></IndividualSwitch>
    </div>
    {/each}
    <img src="/Map.jpg" alt="Map of the railway">
</div>

<style>
    .container{
		display: block;
		position: absolute;
        min-height: 100%;
	}
	.TrackSwitcher{
        position: relative;
        background-color: beige;
        border-radius: 3em;
        padding: 1em;
        max-width: min-content;
        min-width: fit-content;
        margin: auto;
	}
    img{
        border-radius: 3em;
        max-width: 100%;
    }
</style>