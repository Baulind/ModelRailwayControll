<script lang="ts">
    import { onMount } from "svelte";
    import { API } from "../../stores";
    let list: measurement[] = [];
    type measurement = {Id:number, SensorId:number, Speed:Number, time:String};
    
    onMount(async () => {
        fetch($API, {
        headers: {
        "accept": "text/html",
        "Cookie": "Version=1",
        },
        referrerPolicy: "strict-origin-when-cross-origin",
        body: null,
        method: "GET",
        mode: "cors"
        })
        .then(response => response.json())
        .then(result => {
            list = result.data;
            //console.log(temp)
        })
        .catch(error => console.log('error', error));
    })
</script>

<main>
	<ul>
	{#each list as item}
		<li>{item.time}</li>
	{/each}
	</ul>
</main>

<style>

</style>