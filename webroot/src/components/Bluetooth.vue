<template>
<ul>
    <li v-for="device in devices" >{{  device }}</li>
</ul>
</template>

<script setup lang="ts">
import axios from 'axios';
import { ref, onMounted, type Ref } from 'vue'

const devices: Ref<string[]> = ref([]);

onMounted(() => {
    console.log("mounted");
    axios.post('/api/bluetooth/devices')
         .then(response => {devices.value = response.data});
});
</script>

<style scoped>
</style>