<script lang="ts">
	import { each } from "svelte/internal";
	import MotorInput from "./lib/MotorInput.svelte";
    let trains = ["Tog1", "Tog2", "Tog3", "test"];
</script>

{#each trains as train}
    <MotorInput train={train}></MotorInput>
{/each}