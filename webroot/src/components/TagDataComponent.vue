<script setup lang="ts">
import { onMounted, defineProps, ref, type Ref } from "vue";
import type { TagData } from "@/types/TagData";
import { store } from "@/store";
import axios from 'axios';
import { formatTimestamp } from "@/dateformat";
import { type ContentItem } from '@/types/ContentItem';

defineProps<{
  tagData: TagData
}>();

const editMode: Ref<boolean> = ref(false);
const items: Ref<ContentItem[]> = ref([]);

onMounted(() => { 
    axios.get<ContentItem[]>('/api/content/list')
        .then(response => {items.value = response.data});
});

const editAction = () => {
    editMode.value = !editMode.value;
}

const saveAction = () => {
    axios.post('/api/tags/' + store.currentTag.tag, store.currentTag)
         .then(response => {console.log(response);
        editMode.value = false});
}

const cancelAction = () => {
    editMode.value = !editMode.value;
}

const resetPlayStateAction = () => {
    axios.post('/api/tags/' + store.currentTag.tag + '/reset', store.currentTag)
         .then(response => {console.log(response);
        editMode.value = false});
}

</script>

<template>
    <table>
        <tr>
            <td>Tag</td>
            <td>{{ tagData.tag }}</td>
        </tr>
        <tr>
            <td>Name</td>
            <td><span v-if="!editMode">{{ tagData.name }}</span><input v-if="editMode" v-model="tagData.name"/></td>
        </tr>
        <tr>
            <td>Action</td>
            <td><span v-if="!editMode">{{ tagData.action }}</span>
                <select v-if="editMode" v-model="tagData.action">
                    <option disabled value="">Please select one</option>
                    <option value="play_music">Play Music</option>  
                    <option value="none">No Action</option>  
                </select></td>
        </tr>
        <tr>
            <td>Content</td>
            <td><span v-if="!editMode"><router-link :to="`/contentdetails/${tagData.contentRef}`">{{ tagData.contentRef }}</router-link></span><!-- <input v-if="editMode" v-model="tagData.contentRef"/> -->
            <select v-if="editMode" v-model="tagData.contentRef">
                <option v-for="item in items" :item="item" :value="item.id">{{ item.title }}</option>
            </select>
            </td>
        </tr>
        <tr>
            <td>Save State</td>
            <td><span v-if="!editMode">{{ tagData.savePlayState }}</span><input v-if="editMode" type="checkbox" v-model="tagData.savePlayState" /></td>
        </tr>
        <tr>
            <td>Play State</td>
            <td>Index:<span>{{ tagData.playState?.index }}</span> - Position: <span>{{ tagData.playState?.position }}</span> - Seed: <span>{{ tagData.playState?.seed }}</span></td>
        </tr>
        <tr>
            <td>Randomize</td>
            <td><span v-if="!editMode">{{ tagData.randomize }}</span><input v-if="editMode" type="checkbox" v-model="tagData.randomize" /></td>
        </tr>
        <tr>
            <td>Last Seen</td>
            <td><span >{{ tagData.lastSeen?formatTimestamp(tagData.lastSeen) : "never" }}</span></td>
        </tr>
        <tr>
            <td>Play Count</td>
            <td><span >{{ tagData.playCount }}</span></td>
        </tr>
    </table>
    <button @click="editAction" v-if="!editMode">Edit</button>
    <button @click="saveAction" v-if="editMode">Save</button>
    <button @click="cancelAction" v-if="editMode">Cancel</button>
    <button @click="resetPlayStateAction" v-if="editMode">Reset Play State</button>
</template>

<style scoped>

table {
    border: 1px solid black;
}

</style>