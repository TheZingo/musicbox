<script setup lang="ts">
import { type ContentItem } from '@/types/ContentItem';
import axios from 'axios';
import { onMounted, ref, type Ref } from 'vue';
import { useRoute } from 'vue-router'

const route = useRoute();

const contentItem: Ref<ContentItem | undefined> = ref();

onMounted(() => {
    console.log("mounted");
    axios.get('/api/content/'+route.params.contentRef)
         .then(response => {contentItem.value = response.data});
});
</script>

<template>
    <h1>Details: {{ contentItem?.title }}</h1>
    <section class="details">
    <div class="hero-image" v-bind:style="{ backgroundImage: 'url(' + `/api/content/${route.params.contentRef}/cover` + ')' }">
    </div>
    <div>
        {{ contentItem?.description }}
    </div>
    <h2>Files:</h2>
    <ul>
        <li v-for="file in contentItem?.files" :item="file" >{{ file }}</li>
    </ul>
    </section>
</template>

<style scoped>
.hero-image {
    background-position: center;
    background-size: cover;
    height: 37vw;
    max-height: 300px;
    max-width: 900px;
}
.details {
    max-width: 900px;
}
h1, h2 {
    margin-top: 1em;
    margin-bottom: 1em;
}
</style>