import { readable, writable } from 'svelte/store';

//export const Broker = readable("192.168.0.219");
export const Broker = readable("localhost");
export const API = readable("http://localhost:3000/api/values")

export const uuid = writable<string>(crypto.randomUUID().toString());
export const railwayTrack = writable<number[]>(new Array(6).fill(0));
export const switches = writable<boolean[]>(new Array(6).fill(true));
export const switchCords = readable<{x:number, y:number}[]>([
    {x: 5, y: 30}, 
    {x: 15, y: 40},
    {x: 15, y: 70},
    {x: 15, y: 75},
    {x: 20, y: 87},
    {x:77.5, y: 70}
]);

export const indicator = writable<boolean[]>(new Array(28).fill(true));
export const indicatorCords = readable<{x:number, y:number}[]>([
    {x: 45, y: 3}, 
    {x: 45, y: 7}, 
    {x: 64, y: 85},
    {x: 45, y: 83},
    {x: 3, y: 45},
    {x: 11, y: 45},
    {x: 17, y: 58},
    {x: 3, y: 32}, 
    {x: 9, y: 32},
    {x: 14, y: 32},
    {x: 3, y: 22},
    {x: 3, y: 58},
    {x: 11, y: 58},
    {x: 25, y: 22},
    {x: 85, y: 32},
    {x: 78, y: 32},
    {x: 65, y: 32}, 
    {x: 12, y: 69},
    {x: 11, y: 40}, 
    {x: 11, y: 80},
    {x: 15, y: 78}, 
    {x: 19, y: 76},
    {x: 28, y: 85},
    {x: 85, y: 58},
    {x: 78, y: 58},
    {x: 72, y: 58},
    {x: 85, y: 69},
    {x: 78, y: 69}
]);