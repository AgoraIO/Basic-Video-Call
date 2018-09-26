<template>
<div class="dropdown" :class="{'is-active': active}">
    <div class="dropdown-trigger"
      @click="active=!active">
      <a id="advancedProfile" class="ag-rounded button" aria-haspopup="true" aria-controls="advancedOptions">
      <span>Advanced</span>
      </a>
    </div>
    <div class="dropdown-menu" id="advancedOptions" role="menu">
        <div class="dropdown-content">
        <div class="dropdown-item">
            <div class="control">
            <label class="radio">
                <input value="" type="radio" name="transcode" @click="handleRadio" />
                <span>VP8-only</span>
            </label>
            <label class="radio">
                <input value="interop" type="radio" checked @click="handleRadio" name="transcode" />
                <span>VP8 &amp; H264</span>
            </label>
            <label class="radio">
                <input value="h264_interop" type="radio" @click="handleRadio" name="transcode" />
                <span>H264-only</span>
            </label>
            </div>
        </div>
        <div class="dropdown-item">
            <div class="select is-rounded">
            <select @change="handleSelect" value="480p_4" id="videoProfile" class="ag-rounded is-clipped">
              <option v-for="(item, index) in resolutions"
                :key="index" 
                :value="index.split(',')[0]">
                {{item[0]}}x{{item[1]}}, {{item[2]}}fps, {{item[3]}}kbps
              </option>
            </select>
            </div>
        </div>
        </div>
    </div>
</div> 
</template>

<script>
import { RESOLUTION_ARR } from "@/utils/Settings";
export default {
  data() {
    return {
      active: false,
      resolutions: RESOLUTION_ARR
    };
  },

  props: ["onRadioChange", "onSelectChange"],

  methods: {
    handleRadio(e) {
      this.onRadioChange(e.target.value);
    },

    handleSelect(e) {
      this.onSelectChange(e.target.value);
    }
  }
};
</script>

<style scoped>
#advancedOptions .dropdown-content {
  border-radius: 12px;
  border: 1px solid rgb(0, 182, 237);
  background: #004a65;
  text-align: center;
  padding: 12px 3px;
  cursor: pointer;
}

#advancedOptions .dropdown-item:first-child {
  color: white;
  white-space: nowrap;
}

#advancedOptions .radio:hover {
  color: white;
}

#advancedOptions .dropdown-item:nth-child(2) select {
  padding: 0 10px 0 10px;
}

.dropdown.is-active #advancedProfile,
#advancedProfile:hover {
  transition: 0.1s background ease-in-out;
  background: #004a65;
}

#advancedProfile {
  width: 105px;
  font-size: 12px;
  height: 32px;
  color: rgb(0, 182, 237);
  background: transparent;
  border: 1px solid rgb(0, 182, 237);
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


