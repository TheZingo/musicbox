<script setup lang="ts">

import {type Ref, ref } from "vue";
import axios, { type AxiosProgressEvent, type AxiosRequestConfig } from 'axios';

enum States {
  IDLE,
  UPLOADING,
  PROCESSING
}

const fileuploadRef: Ref<HTMLInputElement | undefined> = ref();
const errorList: Ref<string[]> = ref([]);
const status: Ref<States> = ref(States.IDLE);
const progress: Ref<number | undefined> = ref(0);
const fileList: Ref<File[]> = ref([]);

const handleFileUpload = async() => {
  const uploadfileList = fileuploadRef.value?.files;
  if(!uploadfileList || uploadfileList.length <=0) return;
  for(let i = 0; i <uploadfileList.length; i++) {
      fileList.value.push(uploadfileList[i]);
  }
  if(fileuploadRef.value) fileuploadRef.value.value="";
}


const config: AxiosRequestConfig = {
        onUploadProgress: (event: AxiosProgressEvent) => {
          // calculate the upload progress percentage
          let total:number = event.total ?? 1;
          progress.value = Math.round((event.loaded * 100) / total);
          if(progress.value == 100) {
            status.value = States.PROCESSING;
          }
        },
        headers: {
          "Content-Type": "multipart/form-data",
        }
      };

const uploadFile = async() => {    
      if (fileList.value.length > 0 ) {
        // create a form data object with the file
        const formData = new FormData();
        for(const file of fileList.value) {
          formData.append("file", file);
        }
        
        try {
          status.value = States.UPLOADING;
          const response = await axios.post("/api/content/upload", formData, config);
          // handle the response from the server
          console.log(response);
          status.value = States.IDLE;
          fileList.value.splice(0, fileList.value.length);
        } catch (error: unknown) {
          console.error(error);
          errorList.value.push(`${error}`);
        }
      } else {
        errorList.value.push("Please select a file to upload");
      }
    }


const onDrop = (event: DragEvent) => {
  console.log("drop");
  event.preventDefault();
  if(event.dataTransfer) {
    for(let i = 0; i <event.dataTransfer.files.length; i++) {
      fileList.value.push(event.dataTransfer.files[i]);
    }
  }
    
}

const onDragOver = (event: Event) => {
  console.log("drop over");
  event.preventDefault();
    
}


</script>

<template>
    <div class="uploadContainer">
        <div class="uploadArea" v-on:drop="onDrop($event)"  @dragenter.prevent v-on:dragover="onDragOver($event)">
          <div class="icon">
            <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 120 120" style="width: 50px; stroke: grey; fill: grey;"><path d="M73.265 13.79L30.958 56.096l-11.74-11.74 42.308-42.31z"/><path d="M51.943 19.938h19.055v60.21H51.943z"/><path d="M61.454 2l42.326 42.29-11.724 11.74-42.33-42.307zM10 99.575h103v17H10z"/><path d="M6.5 68.576h17v48h-17zm92 0h17v48h-17z"/></svg>
          </div>
          <div v-if="fileList.length > 0" class="fileList">
            Selected files:
            <ul>
              <li v-for="(file) in fileList">{{ file.name }}</li>
            </ul>
          </div>
            
            <input ref="fileuploadRef" id="fileupload" v-on:change="handleFileUpload()" type="file" multiple/>
            <div>Drag & Drop files or <label for="fileupload" class="uploadlink">Select some files</label></div>
            <button class="button" v-on:click="uploadFile()">Upload</button>
            <div v-if="status==States.UPLOADING">
              <p>Uploading....</p>
              <progress :value="progress" max="100"></progress> <span>{{ progress }}%</span>
            </div>
            <div v-if="status==States.PROCESSING">
              <p>Processing....</p>
              <progress></progress>
            </div>
            <div v-if="errorList.length > 0">
                <div
                    class="error"
                    v-for="(error, index) in errorList"
                    :key="index">
                    <div>{{ error }}</div>
                </div>
            </div>
        </div>
    </div>
</template>

<style scoped>

.uploadContainer {
  display: block;
  clear: both;
  margin: 2em auto;
  width: 100%;
  max-width: 600px;
  
}

.uploadArea {
  display: flex;
  flex-direction: column;
  align-items: center;
  min-width: 150px;
  border-radius: 10px;
  border: 3px dashed #eee;
  backdrop-filter: blur(3px);
  transition: transform 250ms;
  background-color: rgba(255, 255, 255, 0.2);
}

.uploadArea:hover {
  border-color: rgba(124, 115, 167, 0.678);
}

.error {
  margin-top: 10px;
  color: #f00;
  font-size: 12px;
}

progress {
  border-radius: 4px;
}

.icon {
  margin: 2em;
}

.button {
  display: inline-block;
  padding: 10px;
  background: #ccc;
  cursor: pointer;
  border-radius: 5px;
  border: 1px solid #ccc;
  margin: 1em;
}
.button:hover {
  background: #ddd;
}

input[type=file] {
  display: none;
}

.uploadlink {
  color: blue;
  text-decoration: underline;
}

.fileList {
  margin-bottom: 1em;
}

</style>