<template>
  <router-view class="full" />
</template>

<script>
export default {
  name: "App"
}
</script>

<style>
/* 

dark cyan #163347;
light blue #00B6ED;
green #7ED321;
pink #E3B6FF;
yellow #F8E71C;
red #D0021B;
dark blue #124A63;
grey #637C8E

*/
html,
body,
#app,
.full {
  height: 100%;
  min-height: 100%;
  font-size: 14px!important;
}

img {
  vertical-align: middle;
  max-width: 100%;
}

.ag-header,
.ag-footer {
  /* flex-basis: 70px; */
  width: 100%;
  opacity: 0.8;
  height: 70px;
  color: #fff;
}

.ag-main {
  width: 100%;
  height: calc(100% - 140px);
  display: flex;
  flex-flow: column;
  justify-content: center;
  align-items: center;
}

.ag-rounded {
  border-radius: 19px !important;
}

.ag-href {
  color: #fff;
  cursor: pointer;
}

.ag-href:hover {
  color: #fff;
}
</style>
