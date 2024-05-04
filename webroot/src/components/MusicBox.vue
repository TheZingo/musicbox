<template>
  <canvas ref="musicboxref" width="320" height="300"> </canvas>
</template>

<script setup lang="ts">
import {onMounted, type Ref, ref, defineProps } from "vue";

const musicboxref: Ref<HTMLCanvasElement | undefined> = ref();
const context: Ref<CanvasRenderingContext2D | undefined> = ref();

const props = defineProps<{
  hasTag?: boolean
}>()


onMounted(() => {    
    context.value = musicboxref.value?.getContext('2d') || undefined;
    window.requestAnimationFrame(animate);
});

var i = 0;
var direction = true;

function animate() {
  if (!context.value) {
      return;
  }
  context.value.clearRect(0, 0, 320, 300);
  context.value.save();
  drawBox(context.value);
  i = direction ? i + 1 : i - 1;
  if (i > 100 && direction) {
    direction = false;
  }
  if (i == 0 && !direction) {
    direction = true;
  }
  if (!props.hasTag) {
    drawArrow(context.value, 105, 40 + i / 10, 100, 80);
  }

  context.value.restore();
  window.requestAnimationFrame(animate);
}

function drawArrow(ctx: CanvasRenderingContext2D, x: number, y: number, w: number, h: number) {
  const w2 = w / 3;
  const h2 = h / 2.5;
  ctx.lineWidth = 5;
  ctx.globalAlpha = 0.3;
  ctx.beginPath();
  ctx.moveTo(x + w2, y);
  ctx.lineTo(x + 2 * w2, y);
  ctx.lineTo(x + 2 * w2, y + h2);
  ctx.lineTo(x + w, y + h2);
  ctx.lineTo(x + w / 2, y + h);
  ctx.lineTo(x, y + h2);
  ctx.lineTo(x + w2, y + h2);
  ctx.closePath();
  ctx.fillStyle = "#ffffff"; // Set the fill style to white
  ctx.fill();
  ctx.globalAlpha = 1.0;
  ctx.strokeStyle = "rgba(124, 115, 167, 0.678)";
  ctx.stroke();
}



function drawBox(ctx: CanvasRenderingContext2D) {
  var h = 70;
  var x = 10;
  var y = 150;
  ctx.strokeStyle = "rgba(124, 115, 167, 0.678)";
  ctx.lineWidth = 5;
  ctx.lineJoin = "round";
  ctx.beginPath();
  ctx.moveTo(x, y);
  ctx.lineTo(x, y + h);
  ctx.lineTo(x + 140, y + 100);
  ctx.lineTo(x + 140, y + 100 - h);
  ctx.closePath();

  var gradX = x + 140 / 2;
  var gradY = y + 12;
  var gradXOff = -15;
  var gradYOff = h;
  const a1 = ctx.createLinearGradient(
    gradX,
    gradY,
    gradX + gradXOff,
    gradY + gradYOff
  );
  a1.addColorStop(0, "orange");
  a1.addColorStop(0.5, "orange");
  a1.addColorStop(1, "rgba(255,255,255,0)");
  ctx.fillStyle = a1;
  ctx.fill();
  ctx.beginPath();
  ctx.moveTo(gradX, gradY);
  ctx.lineTo(gradX + gradXOff, gradY + gradYOff);
  

  ctx.beginPath();
  ctx.moveTo(x + 150, y + 100);
  ctx.lineTo(x + 150, y + 100 - h);
  ctx.lineTo(x + 150 + 140, y);
  ctx.lineTo(x + 150 + 140, y + h);
  ctx.closePath();
  

  gradX = x + 150 + 140 / 2;
  gradY = y + 12;
  gradXOff = 15;
  gradYOff = h;
  const a = ctx.createLinearGradient(
    gradX,
    gradY,
    gradX + gradXOff,
    gradY + gradYOff
  );
  a.addColorStop(0, "orange");
  a.addColorStop(0.5, "orange");
  a.addColorStop(1, "rgba(255,255,255,0)");
  ctx.fillStyle = a;
  ctx.fill();

  ctx.beginPath();
  ctx.moveTo(gradX, gradY);
  ctx.lineTo(gradX + gradXOff, gradY + gradYOff);

  ctx.beginPath();
  ctx.moveTo(x, y - 10);
  ctx.lineTo(x + 145, y + 100 - h - 10);
  ctx.lineTo(x + 140 + 140 + 10, y - 10);
  ctx.lineTo(x + 145, y - 40);
  ctx.closePath();
  if (!props.hasTag) {
    ctx.fillStyle = "orange";
  } else {
    ctx.fillStyle = "green";
  }
  ctx.fill();
  ctx.beginPath();
  ctx.ellipse(153, 140, 15, 50, Math.PI / 2, 0, 2 * Math.PI);
  ctx.strokeStyle = "rgba(255,255,255,0.5)";
  ctx.stroke();
}

</script>


<style>

</style>