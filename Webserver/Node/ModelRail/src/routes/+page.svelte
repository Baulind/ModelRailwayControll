<script lang="ts">
	import MotorInput from './lib/MotorInput.svelte';
	let trains = ['Tog1'];
</script>

{#each trains as train}
	<MotorInput {train} />
{/each}
