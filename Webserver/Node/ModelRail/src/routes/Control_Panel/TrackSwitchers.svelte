<script lang="ts">
	import { switches, switchCords, indicator, indicatorCords } from "../stores";
	import IndividualSwitch from "./IndividualSwitch.svelte";
    import { createEventDispatcher } from 'svelte';
    
	const dispatch = createEventDispatcher();
    function handleSwitch(event: CustomEvent<any>){
        dispatch('setSwitch', {
			id: event.detail.id,
			value: event.detail.value
		});
    }
</script>

<div class="TrackSwitcher">
    {#each $indicator || [] as Indicator, i}
    <div class="container" style="left: {$indicatorCords[i].x}%; top: {$indicatorCords[i].y}%;">
        {#if Indicator === true}
        <p>🚂</p>
        {/if}
    </div>
    {/each}
    {#each $switches || [] as Switch, i}
    <div class="container" style="left: {$switchCords[i].x}%; top: {$switchCords[i].y}%;">
        <IndividualSwitch Id={i} bind:Value={Switch} on:setSwitch={handleSwitch}/>
    </div>
    {/each}
    <img src="/Map.jpg" alt="Map of the railway">
</div>

<style>
    .container{
		display: block;
		position: absolute;
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
        max-height: 50em;
    }
    p{
        font-size: x-large;
    }
</style>