<script setup lang="ts">
import ContentItemComponent from './ContentItemComponent.vue';
import { type ContentItem } from '@/types/ContentItem';
import axios from 'axios';
import { ref } from 'vue';
</script>

<template>
    <div class="content-elements">
        <ContentItemComponent v-for="item in items" :item="item" />
    </div>
</template>


<script lang="ts">
export default {
    data() {
        return {
            items: [] as ContentItem[]
        };
    },
    mounted() {
          axios.get<ContentItem[]>('/api/content/list')
        .then(response => {this.items = response.data});
    }

}
</script>

<style scoped>
.content-elements {
    display: flex;
    flex-wrap: wrap;
    gap: 1em;
    padding: 5px 0px;
    width: 100%;
}
</style>