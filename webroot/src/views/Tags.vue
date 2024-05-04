<script setup lang="ts">
import type { TagData } from '@/types/TagData';
import axios from 'axios';
import { onMounted, ref, type Ref } from 'vue';
import { useRoute } from 'vue-router'
import { formatTimestamp } from '@/dateformat';

const route = useRoute();

const tags: Ref<TagData[]> = ref([]);

onMounted(() => {
    console.log("mounted");
    axios.get('/api/tags')
         .then(response => {tags.value = response.data});
});
</script>

<template>
    <h1>Tags</h1>
    <table>
        <tr>
            <th>Tag Name</th>
            <th>ID</th>
            <th>Content</th>
            <th>Last Seen</th>
        </tr>
        <tr v-for="tag in tags" :item="tag">
            <td>{{ tag.name }}</td>
            <td>{{ tag.tag }}</td>
            <td>{{ tag.contentRef }}</td>
            <td>{{ tag.lastSeen ? formatTimestamp(tag.lastSeen) : "never" }}</td>
        </tr>
    </table>
</template>

<style scoped>
table {
    border-collapse: collapse;
}
td, th {
  border: 1px solid #ddd;
  padding: 8px;
}
th {
  padding-top: 12px;
  padding-bottom: 12px;
  text-align: left;
  background-color: #3670b3;
  color: white;
}
tr:nth-child(even){background-color: #f2f2f2;}
</style>