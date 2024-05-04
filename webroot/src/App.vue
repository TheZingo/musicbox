<script setup lang="ts">
import PlayerControls from './components/PlayerControls.vue'
import { store } from './store'


function createWebSocket(path: string) {
  var protocolPrefix = window.location.protocol === "https:" ? "wss:" : "ws:";
  return new WebSocket(protocolPrefix + "//" + location.host + path);  
}

const socket = createWebSocket("/api/live");

socket.onopen = function (e) {
  console.log("[open] Connection established");
};

socket.onmessage = function (event) {
  console.log(`[message] Data received from server: ${event.data}`);
  let data = JSON.parse(event.data);
  if (data.event == "tag_unknown") {
    store.hasTag = true;
    store.currentTag = { tag: data.id };
  }
  if (data.event == "tag_found") {
    store.hasTag = true;
    store.currentTag = data.tagData;
  }
  if (data.event == "tag_removed") {
   store.hasTag = false;
   store.currentTag = {tag: ""};
  }
  if(data.event == "play") {
    store.playState = true;
  }
  if(data.event == "stop") {
    store.playState = false;
  }
};

socket.onclose = function (event) {
  if (event.wasClean) {
    console.log(
      `[close] Connection closed cleanly, code=${event.code} reason=${event.reason}`
    );
  } else {
    console.log("[close] Connection died");
  }
};

socket.onerror = function (error) {
  console.log(`[error]`);
};

const togglePlay = async() => {  
  if(!store.playState) {
    socket.send("startPlay");
  } else {
    socket.send("stopPlay");
  }
  store.playState = !store.playState;
}

const backwardAction = async() => {
  socket.send("backward");
}

const forwardAction = async() => {
  socket.send("forward");
}

const seekBackwardsAction = async() => {
  socket.send("seekBackward");
}

const seekForwardAction = async() => {
  socket.send("seekForward");
}

</script>

<template>
  <main>
    <nav>
      <div class="sticky">
        <div class="wrapper text">
          <img alt="Musicbox logo" class="logo" src="./assets/logo.png" width="125" />
          <h1>MusicBox</h1>
        </div>
        <ul>
          <li>
            <router-link to="/">
              <div class="icon">
                <svg width="30px" height="30px" viewBox="0 0 237 237" version="1.1" xmlns="http://www.w3.org/2000/svg"
                  xmlns:xlink="http://www.w3.org/1999/xlink" xml:space="preserve" xmlns:serif="http://www.serif.com/"
                  style="fill-rule:evenodd;clip-rule:evenodd;stroke-linejoin:round;stroke-miterlimit:2;">
                  <g>
                    <path
                      d="M186.181,95.795L186.181,217.185C186.181,221.097 184.626,224.849 181.86,227.615C179.094,230.381 175.342,231.935 171.43,231.935L64.791,231.935C60.879,231.935 57.127,230.381 54.36,227.615C51.594,224.849 50.04,221.097 50.04,217.185L50.04,95.795L186.181,95.795Z" />
                    <path
                      d="M112.758,6.517C114.173,5.089 116.1,4.285 118.11,4.285C120.121,4.285 122.047,5.089 123.462,6.517C138.741,21.944 182.87,66.502 204.82,88.665C206.959,90.825 207.59,94.059 206.42,96.865C205.25,99.67 202.508,101.498 199.468,101.498C161.66,101.498 74.561,101.498 36.752,101.498C33.712,101.498 30.97,99.67 29.8,96.865C28.63,94.059 29.261,90.825 31.4,88.665C53.35,66.502 97.48,21.944 112.758,6.517Z" />
                  </g>
                </svg>
              </div>
              <div class="text">Home</div>
            </router-link>
          </li>
          <li>
            <router-link to="/content">
              <div class="icon"><svg width="30px" height="30px" viewBox="0 0 237 237" version="1.1"
                  xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" xml:space="preserve"
                  xmlns:serif="http://www.serif.com/"
                  style="fill-rule:evenodd;clip-rule:evenodd;stroke-linecap:round;stroke-linejoin:round;stroke-miterlimit:1.5;">
                  <g>
                    <circle cx="179.28" cy="169.387" r="29.629" style="stroke:white;stroke-width:2.27px;" />
                    <circle cx="56.941" cy="169.387" r="29.629" style="stroke:white;stroke-width:2.27px;" />
                    <rect x="71.541" y="56.355" width="13.029" height="117.982" />
                    <path
                      d="M206.909,65.798L71.541,65.798L71.541,54.039C71.541,52.148 72.292,50.336 73.628,48.999C74.965,47.662 76.778,46.911 78.668,46.911C102.068,46.911 176.381,46.911 199.781,46.911C201.672,46.911 203.485,47.662 204.821,48.999C206.158,50.336 206.909,52.148 206.909,54.039C206.909,59.39 206.909,65.798 206.909,65.798Z" />
                    <rect x="193.62" y="56.355" width="13.288" height="117.982" />
                  </g>
                </svg></div>
              <div class="text">Content</div>
            </router-link>
          </li>
          <li><router-link to="/tags"><div class="icon"><svg width="30px" height="30px" viewBox="0 0 237 237" version="1.1"
                xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" xml:space="preserve"
                xmlns:serif="http://www.serif.com/"
                style="fill-rule:evenodd;clip-rule:evenodd;stroke-linecap:round;stroke-linejoin:round;stroke-miterlimit:1.5;">
                <g>
                  <path
                    d="M21.471,211.57L21.471,191.616L223.253,191.616L223.253,211.57C223.253,220.581 178.045,227.896 122.362,227.896C66.678,227.896 21.471,220.581 21.471,211.57Z" />
                  <ellipse cx="122.362" cy="191.616" rx="100.891" ry="16.326"
                    style="stroke:white;stroke-width:2.27px;" />
                  <path
                    d="M71.466,139.588C75.248,140.277 76.194,152.487 73.576,166.836C70.959,181.186 65.763,192.275 61.981,191.585C58.199,190.895 57.253,178.685 59.871,164.336C62.488,149.987 67.684,138.898 71.466,139.588Z"
                    style="stroke:white;stroke-width:2.27px;" />
                  <ellipse cx="124.514" cy="135.284" rx="64.618" ry="42.792"
                    style="stroke:white;stroke-width:2.27px;" />
                  <ellipse cx="156.823" cy="181.09" rx="32.309" ry="13.312" style="stroke:white;stroke-width:2.27px;" />
                  <ellipse cx="178.301" cy="155.711" rx="10.831" ry="24.75" style="stroke:white;stroke-width:2.27px;" />
                  <path
                    d="M79.799,141.975C83.582,142.665 84.527,154.875 81.91,169.224C79.292,183.573 74.096,194.663 70.314,193.973C66.532,193.283 65.587,181.073 68.204,166.724C70.822,152.375 76.017,141.285 79.799,141.975Z"
                    style="stroke:white;stroke-width:2.27px;" />
                  <ellipse cx="64.901" cy="89.763" rx="31.175" ry="32.374" style="stroke:white;stroke-width:2.27px;" />
                  <ellipse cx="52.93" cy="38.197" rx="6.966" ry="29.873" style="stroke:white;stroke-width:2.27px;" />
                  <ellipse cx="72.635" cy="38.197" rx="6.966" ry="29.873" style="stroke:white;stroke-width:2.27px;" />
                </g>
              </svg></div>
              <div class="text">Tags</div></router-link></li>
          <li><router-link to="/bluetooth"><div class="icon"><svg width="30px" height="30px" viewBox="0 0 237 237" version="1.1"
                xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" xml:space="preserve"
                xmlns:serif="http://www.serif.com/"
                style="fill-rule:evenodd;clip-rule:evenodd;stroke-linejoin:round;stroke-miterlimit:2;">
                <clipPath id="_clip1">
                  <rect id="bluetooth.svg" x="7.034" y="7.034" width="222.153" height="222.153" />
                </clipPath>
                <g clip-path="url(#_clip1)">
                  <g>
                    <path
                      d="M132.935,118.11L179.508,72.67C181.281,70.94 182.281,68.568 182.281,66.091C182.281,63.614 181.281,61.241 179.508,59.512L128.399,9.647C125.753,7.065 121.817,6.323 118.41,7.755C115.003,9.191 112.787,12.529 112.787,16.226L112.787,98.453L69.55,56.269C65.916,52.724 60.097,52.794 56.551,56.428C53.006,60.063 53.077,65.883 56.711,69.428L106.608,118.111L56.711,166.794C53.077,170.339 53.006,176.159 56.551,179.793C60.096,183.427 65.917,183.499 69.55,179.952L112.787,137.769L112.787,219.995C112.787,223.693 115.003,227.031 118.41,228.467C119.562,228.953 120.775,229.188 121.978,229.188C124.332,229.188 126.648,228.283 128.399,226.574L179.508,176.709C181.281,174.98 182.281,172.608 182.281,170.131C182.281,167.654 181.281,165.281 179.508,163.552L132.935,118.11ZM131.173,38.037L159.926,66.09L131.173,94.143L131.173,38.037ZM131.173,198.181L131.173,142.076L159.926,170.129L131.173,198.181Z"
                      style="fill-rule:nonzero;" />
                  </g>
                </g>
              </svg></div>
              <div class="text">Bluetooth</div></router-link></li>
        </ul>
      </div>
    </nav>

    <section>
      <router-view />
      <PlayerControls :playState="store.playState" @togglePlay="togglePlay" @backward="backwardAction"
        @forward="forwardAction" @seekForward="seekForwardAction" @seekBackward="seekBackwardsAction" />
    </section>
  </main>
</template>

<style scoped>

main {
  display: flex;
  justify-content: space-between;
}

nav {
  background: rgb(52, 120, 223);
  width: min(300px, 13%);
  min-height: 100vh;
}

section {
  flex-grow: 1;
}

.text {
  text-align: center;
}

.sticky {
  position: -webkit-sticky;
  position: sticky;
  top: 0;
}

nav ul,
nav ul li {
  outline: 0;
  padding-left: 10px;
}

nav ul li a {
  text-decoration: none;
}

li {
  position: relative;
  display: block;
}
li a {
  position: relative;
  display: flex;
  flex-direction: row;
  align-items: center;
  justify-content: center;
  color: #fff;
  font-size: 1rem;
  padding: 15px 0;
  border-top-left-radius: 20px;
  border-bottom-left-radius: 20px;
}

.router-link-active {
  text-decoration: none;
  color: #000;
  background: rgba(251,207,198,1);
}

section {
  margin-left:2em;
}

@media (max-width: 1500px) {

  nav {
    min-width: 50px;
  }
  .logo {
    display: none;
  }
  .text {
    display: none;
  }
  nav ul {
    padding-left: 0px;
  }
  nav ul li {
    padding-left: 0px;
  }
}
</style>
