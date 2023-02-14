<script lang="ts">
	//export let Id: number;
	export let Value: boolean = false;
	export let Id: number = 0;
	import { createEventDispatcher } from 'svelte';
	const dispatch = createEventDispatcher();
	function setSwitch(){
		dispatch('setSwitch', {
			id: Id,
			value: Value
		});
	}
</script> 

<label>
	<input type="checkbox" bind:checked={Value} on:change={setSwitch}>
	<span class="checkmark"></span>
</label>

<style>
	/* Customize the label (the container) */

	label{
		cursor: pointer;
		-webkit-user-select: none;
		-moz-user-select: none;
		-ms-user-select: none;
		user-select: none;
	}
	/* Hide the browser's default checkbox */
	input {
	position: absolute;
	opacity: 0;
	cursor: pointer;
	height: 0;
	width: 0;
	}

	/* Create a custom checkbox */
	.checkmark {
	position: absolute;
	height: 2em;
	width: 2em;
	border-radius: 2em;
	border: black;
	background-color: #e60707;
	}

	/* When the checkbox is checked, add a blue background */
	input:checked ~ .checkmark {
		background-color: #25f321;
	}


</style>
