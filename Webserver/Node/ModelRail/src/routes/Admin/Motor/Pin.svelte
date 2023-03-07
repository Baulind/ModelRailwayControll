<script lang="ts">
    export let Output: number;
    export let Direction: number;
    export let Id: Number = 0;
    const max: number = 34;
    const min: number = 1;
    $:{
        if (Output > max) {
            Output = max;
        } 
        else if (Output < min) {
            Output = min;
        }
    }
    $:{
        if (Direction > max) {
            Direction = max;
        } 
        else if (Direction < min) {
            Direction < min;
        }
    }
</script>

<div class="configureMotor shape">
    <div>
        <span>{Id}# </span>
        <span>Output: </span>
        <span><input class="editable" type="number" min={min} max={max} bind:value={Output}></span>
        <span>Direction: </span>
        <span><input class="editable" type="number" min={min} max={max} bind:value={Direction}></span>
    </div>
</div>

<style>
    .editable{
        width: min-content;
    }
    .shape{
        border-radius: 3em;
    }
    .configureMotor{
        padding: 1.5em;
        margin: 0.5em;
        display: flex;
        flex-direction: column;
        background-color: beige;
        gap: 1em;
        width: max-content;
        height: max-content;
    }
</style>