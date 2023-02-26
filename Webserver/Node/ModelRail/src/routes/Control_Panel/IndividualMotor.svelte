<script lang="ts">
	//export let Id: number;
	export let Value: number = 0;
	export let Id: number = 0;
	import { createEventDispatcher } from 'svelte';
	const dispatch = createEventDispatcher();
	function setSpeed(speed: number){
		dispatch('setSpeed', {
			id: Id,
			value: speed
		});
	}
	$: setSpeed(Value);
</script> 

<div class="slidecontainer">
	<input
	type="range"
	min={-255}
	max={255}
	bind:value={Value}
	class="slider"
	/>
	<button
	class="resetButton" 
	on:click={() => {Value = 0}}
	>❌</button>	
</div>
	
<style>
		/*clip-path: polygon(0% 0%, 50% 45%, 100% 0%, 100% 100%, 50% 55%, 0% 100%);*/
	.slider {
		outline: none;
		transform: rotate(-90deg);
	}
	.slidecontainer {
		display: flex;
		max-width: fit-content;
		margin: auto;
		padding: 4em 0;
		border-radius: 3em;
		justify-content: center;
	}
	.resetButton{
		font-size: 4em;
		border: none;
		background: none;
		text-shadow: 1px 1px 2px black;
		cursor: pointer;
		
	}
	button:active{
		transform: translateY(3px);
		text-shadow: none;
	}
</style>
