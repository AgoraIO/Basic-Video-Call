<template>
  <div class="dropdown" :class="{'is-active': active}">
    <div class="dropdown-trigger" 
      @click="active=!active">
      <a id="baseMode" class="ag-rounded button" aria-haspopup="true" aria-controls="baseModeOptions">
        <span id="baseOptionLabel">{{message}}</span>
        <span class="icon is-small">
          <i class="ag-icon ag-icon-arrow-down" aria-hidden="true"></i>
        </span>
      </a>
    </div>
    <div class="dropdown-menu" id="baseModeOptions" role="menu">
      <div class="dropdown-content">
        <div v-for="(item, index) in options"
          class="dropdown-item" 
          :key="index"
          @click="handleSelect(item)">
          <p>{{item.label}}</p>
          <hr />
          <p>{{item.content}}</p>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  data() {
    return {
      options: [
        {
          label: "Agora Video Call",
          value: "avc",
          content: "One to one and group calls"
        },
        {
          label: "Agora Live",
          value: "al",
          content:
            "Enabling real-time interactions between the host and the audience"
        }
      ],
      active: false,
      message: "Agora Video Call"
    };
  },
  methods: {
    handleSelect(item) {
      let msg = item.label;
      let val = item.value;
      this.message = msg;
      this.active = false;
      this.$emit("change", val);
    }
  }
};
</script>

<style scoped>
#baseMode {
  font-size: 14px;
  width: 160px;
  border: 1px solid transparent !important;
}

#baseModeOptions .dropdown-item {
  text-align: center;
  padding: 8px;
  cursor: pointer;
}

#baseModeOptions .dropdown-item:hover {
  background: #e8ecee;
}
#baseModeOptions .dropdown-item p:first-child {
  font-size: 14px;
  font-weight: bold;
  color: #163347;
}
#baseModeOptions .dropdown-item:hover p:first-child {
  color: #00b6ed;
}
#baseModeOptions .dropdown-item hr {
  margin: 10px;
}
#baseModeOptions .dropdown-item p:nth-child(3) {
  font-size: 12px;
  color: rgba(0, 0, 0, 0.5);
}
.dropdown.is-active .dropdown-menu {
  animation: dropdown 0.25s ease-in-out;
}

@keyframes dropdown {
  from {
    opacity: 0;
    transform: scale(0.92) translate(0, -4px);
  }
  to {
    opacity: 1;
    transform: scale(none) translate(none);
  }
}
</style>

