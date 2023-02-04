<script lang="ts">
	import Slider from "./Slider.svelte";
    import type { Motor } from "src/types/Motor.type";
    export let train: string;
    const thing: Motor = {id:1, min:-255, max:255, val:0, motor:false};
</script>

<h2>{train}</h2>
<div class="container">
    <h3>Motor</h3>
    <h3>Hastighet</h3>
    <Slider motor={thing}></Slider>
</div>

 <style>
    .container{
        display: grid;
        gap: 1rem;
        padding: 0.5rem;
        background-color: rgb(161, 161, 161);
        grid-template-columns: 5rem 10rem;
        grid-template-rows: repeat(2, auto);
    }
 </style>