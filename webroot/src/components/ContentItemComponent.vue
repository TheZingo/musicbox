<script setup lang="ts">
import { defineProps } from "vue";
import type { ContentItem } from "@/types/ContentItem";
defineProps<{
  item: ContentItem
}>();
const emits = defineEmits<{
  (e: "selected", path: string): void;
}>();

function select(path: string) {
  console.log("select: " + path);
  emits("selected", path);
}
</script>

<template>
    <div class="content-item">
        
        <div class="image">
          <router-link :to="`/contentdetails/${item.id}`"><img :src="'/api/content/' + item.id +'/cover'" /></router-link>
        </div>
        <span class="content-type">{{ item.type}}</span>
        <h2><router-link :to="`/contentdetails/${item.id}`">{{  item.title }}</router-link></h2>
    </div>
</template>

<style scoped>
.content-item {
  position: relative;
    width: 320px;    
    border-radius: 10px;
    backdrop-filter: blur(3px);
    border: 1px solid rgba(255, 255, 255, 0.1);
    transition: transform 250ms;
    background-color: rgba(255, 255, 255, 0.2);
}
.content-type {
  position: absolute;
  top: 10px;
  background-color: red;
  color:white;
  font-size: 0.8em;
  text-transform: uppercase;
  padding: 5px;
  border-top-right-radius: 5px;
  border-bottom-right-radius: 5px;
}

.image {
  overflow: hidden;
  width: 320px;
  height: 320px;
}
.image img {
  display: block;
  width: 100%;
  height: 100%;
  object-fit: cover;
  border-radius: 6px;
}
.content-item:hover {
    transform: translateY(-5px);
  }

.content-item a {
  text-decoration: none;
}
</style>