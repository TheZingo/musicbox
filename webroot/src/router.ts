import { createWebHistory, createRouter } from "vue-router";
import Home from '@/views/Home.vue'
import Bluetooth from '@/views/Bluetooth.vue'
import Content from '@/views/Content.vue'
import Tags from '@/views/Tags.vue'
import ContentDetails from "./views/ContentDetails.vue";

export default createRouter({
    history: createWebHistory(),
    routes: [
        {
            path: '/',
            component: Home
        },
        {
            path: '/content',
            component: Content
        },
        {
            path: '/contentdetails/:contentRef',
            component: ContentDetails
        },
        {
            path: '/tags',
            component: Tags
        },
        {
            path: '/bluetooth',
            component: Bluetooth
        }
    ]
})