<template>
  <div class="controlscontainer">
    <button @click="seekBackward"><svg width="100%" height="100%" viewBox="0 0 183 141" version="1.1" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" xml:space="preserve" xmlns:serif="http://www.serif.com/" style="fill-rule:evenodd;clip-rule:evenodd;stroke-linejoin:round;stroke-miterlimit:2;">
    <path d="M18.028,93.898C18.028,93.898 10.124,95.039 4.783,95.81C4.04,95.917 3.442,96.475 3.283,97.209C3.125,97.943 3.439,98.698 4.072,99.103C13.314,105.009 35.498,119.188 40.958,122.678C41.675,123.136 42.615,123.029 43.21,122.421C47.908,117.624 67.567,97.555 75.832,89.117C76.37,88.568 76.499,87.737 76.153,87.051C75.807,86.364 75.062,85.974 74.301,86.08C68.195,86.929 58.907,88.221 58.907,88.221C58.646,86.339 58.515,84.442 58.515,82.543C58.515,59.765 77.008,41.272 99.787,41.272C122.565,41.272 141.058,59.765 141.058,82.543C141.058,92.908 137.159,102.871 130.174,110.471C129.513,111.176 129.531,112.278 130.214,112.961C134.968,117.724 152.039,134.795 156.871,139.627C157.211,139.968 157.675,140.158 158.157,140.154C158.639,140.15 159.099,139.953 159.434,139.606C174.111,124.26 182.33,103.825 182.33,82.543C182.33,36.986 145.343,0 99.787,0C54.23,0 17.244,36.986 17.244,82.543C17.244,86.342 17.506,90.136 18.028,93.898Z"/>
</svg></button>
    <button @click="backward"><svg width="100%" height="100%" viewBox="0 0 179 161" version="1.1" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" xml:space="preserve" xmlns:serif="http://www.serif.com/" style="fill-rule:evenodd;clip-rule:evenodd;stroke-linejoin:round;stroke-miterlimit:2;">
    <g>
        <path d="M173.598,3.811C174.603,3.081 175.933,2.976 177.039,3.54C178.146,4.104 178.843,5.241 178.843,6.484C178.843,32.23 178.843,127.118 178.843,153.323C178.843,154.584 178.126,155.735 176.993,156.29C175.861,156.844 174.511,156.706 173.515,155.934C154.648,141.302 91.315,92.189 75.748,80.117C74.927,79.48 74.453,78.494 74.47,77.455C74.486,76.415 74.99,75.444 75.831,74.833C91.561,63.406 154.867,17.418 173.598,3.811Z"/>
        <path d="M101.327,3.811C102.332,3.081 103.662,2.976 104.769,3.54C105.876,4.104 106.573,5.241 106.573,6.484C106.573,32.23 106.573,127.118 106.573,153.323C106.573,154.584 105.855,155.735 104.723,156.29C103.59,156.844 102.241,156.706 101.244,155.934C82.377,141.302 19.044,92.189 3.478,80.117C2.657,79.48 2.183,78.494 2.199,77.455C2.215,76.415 2.72,75.444 3.561,74.833C19.291,63.406 82.596,17.418 101.327,3.811Z"/>
    </g>
</svg></button>
    <button @click="togglePlay" class="playpause"><svg v-if="playState"width="100%" height="100%" viewBox="0 0 154 154" version="1.1" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" xml:space="preserve" xmlns:serif="http://www.serif.com/" style="fill-rule:evenodd;clip-rule:evenodd;stroke-linejoin:round;stroke-miterlimit:2;">
    <path d="M153.426,6.396L153.426,147.031C153.426,150.561 150.561,153.426 147.031,153.426L6.396,153.426C2.866,153.426 0,150.561 0,147.031L0,6.396C0,2.866 2.866,0 6.396,0L147.031,0C150.561,0 153.426,2.866 153.426,6.396Z"/>
</svg>
<svg v-if="!playState" width="100%" height="100%" viewBox="0 0 107 161" version="1.1" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" xml:space="preserve" xmlns:serif="http://www.serif.com/" style="fill-rule:evenodd;clip-rule:evenodd;stroke-linejoin:round;stroke-miterlimit:2;">
    <path d="M5.246,3.811C4.241,3.081 2.911,2.976 1.804,3.54C0.697,4.104 0,5.241 0,6.484C-0,32.23 -0,127.118 0,153.323C-0,154.584 0.718,155.735 1.85,156.29C2.983,156.844 4.332,156.706 5.329,155.934C24.196,141.302 87.529,92.189 103.095,80.117C103.916,79.48 104.39,78.494 104.374,77.455C104.358,76.415 103.853,75.444 103.012,74.833C87.282,63.406 23.977,17.418 5.246,3.811Z"/>
</svg>
      </button>
    <button @click="forward"><svg width="100%" height="100%" viewBox="0 0 179 161" version="1.1" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" xml:space="preserve" xmlns:serif="http://www.serif.com/" style="fill-rule:evenodd;clip-rule:evenodd;stroke-linejoin:round;stroke-miterlimit:2;">
    <g>
        <path d="M5.246,3.811C4.241,3.081 2.911,2.976 1.804,3.54C0.697,4.104 0,5.241 0,6.484C0,32.23 0,127.118 0,153.323C-0,154.584 0.718,155.735 1.85,156.29C2.983,156.844 4.332,156.706 5.329,155.934C24.196,141.302 87.529,92.189 103.095,80.117C103.916,79.48 104.39,78.494 104.374,77.455C104.358,76.415 103.853,75.444 103.012,74.833C87.282,63.406 23.977,17.418 5.246,3.811Z"/>
        <path d="M77.516,3.811C76.511,3.081 75.181,2.976 74.074,3.54C72.967,4.104 72.27,5.241 72.27,6.484C72.27,32.23 72.27,127.118 72.27,153.323C72.27,154.584 72.988,155.735 74.121,156.29C75.253,156.844 76.602,156.706 77.599,155.934C96.466,141.302 159.799,92.189 175.365,80.117C176.187,79.48 176.661,78.494 176.644,77.455C176.628,76.415 176.123,75.444 175.283,74.833C159.553,63.406 96.247,17.418 77.516,3.811Z"/>
    </g>
</svg></button>
<button @click="seekForward"><svg width="100%" height="100%" viewBox="0 0 183 141" version="1.1" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" xml:space="preserve" xmlns:serif="http://www.serif.com/" style="fill-rule:evenodd;clip-rule:evenodd;stroke-linejoin:round;stroke-miterlimit:2;">
    <path d="M164.301,93.898C164.301,93.898 172.206,95.039 177.547,95.81C178.29,95.917 178.888,96.475 179.046,97.209C179.205,97.943 178.89,98.698 178.258,99.103C169.016,105.009 146.832,119.188 141.371,122.678C140.655,123.136 139.715,123.029 139.12,122.421C134.421,117.624 114.763,97.555 106.497,89.117C105.959,88.568 105.83,87.737 106.176,87.051C106.523,86.364 107.267,85.974 108.029,86.08C114.135,86.929 123.422,88.221 123.422,88.221C123.683,86.339 123.815,84.442 123.815,82.543C123.815,59.765 105.321,41.272 82.543,41.272C59.765,41.272 41.272,59.765 41.272,82.543C41.272,92.908 45.17,102.871 52.156,110.471C52.817,111.176 52.799,112.278 52.116,112.961C47.362,117.724 30.291,134.795 25.459,139.627C25.118,139.968 24.655,140.158 24.173,140.154C23.691,140.15 23.231,139.953 22.896,139.606C8.219,124.26 0,103.825 0,82.543C0,36.986 36.986,0 82.543,0C128.1,0 165.086,36.986 165.086,82.543C165.086,86.342 164.824,90.136 164.301,93.898Z"/>
</svg></button>
  </div>
</template>

<script setup lang="ts">

defineProps<{
  playState: boolean
}>();

const emit = defineEmits(['togglePlay', 'backward', 'forward', 'seekForward', 'seekBackward'])

const togglePlay = async () => {
  emit("togglePlay");
}

const forward = async () => {
  emit("forward");
}

const backward = async () => {
  emit("backward");
}

const seekForward = async () => {
  emit("seekForward");
}

const seekBackward = async () => {
  emit("seekBackward");
}

</script>

<style scoped>

.controlscontainer {
    
    display: flex;
    align-items: center;
    justify-content: center;
    flex-direction: row;
    gap: .1em;
}

button {    
    width: 50px;
    aspect-ratio: 1/1;
    border-radius: 50%;
    background: rgba(163,162,164,0.3);
    margin: 10px;
    border: 1px solid rgba(0, 0, 0, 0.384);
    cursor: pointer;
    display: flex;
    align-items: center;
    justify-content: center;
    outline: 0;
    transition: all 0.3s linear;
}

button svg {
  width: 50%;
  height: 50%;
}

button:hover {
  background: rgba(163, 162, 164, 0.705);
}

.playpause {
  transform: scale(1.3);
}
</style>