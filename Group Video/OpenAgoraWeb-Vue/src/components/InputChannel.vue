<template>
	<div class="channel-wrapper control has-icons-left">
		<input 
			@input="handleChange" 
			id="channel" 
			class="ag-rounded input"
			:class="state"
			type="text" 
			:placeholder="placeholder" />
		<span class="icon is-small is-left">
		<img :src="require('../assets/images/ag-login.png')" alt="" />
		</span>
		<span class="validate-icon">
      <i v-if="state" class="ag-icon" :class="{'ag-icon-valid': state === 'is-success', 'ag-icon-invalid': state === 'is-danger'}" />           
		</span>
		<div class="validate-msg">
      {{errorMsg}}
		</div>
	</div>
</template>

<script>
import Validator from "@/utils/Validator";
export default {
  data() {
    return {
      state: "",
      errorMsg: ""
    };
  },

  props: ["placeholder"],

  methods: {
    validate(val) {
      this.state = "";
      this.errorMsg = "";
      if (Validator.isNonEmpty(val.trim())) {
        (this.errorMsg = "Cannot be empty!"), (this.state = "is-danger");
        return false;
      } else if (Validator.minLength(val.trim(), 1)) {
        (this.errorMsg = "No shorter than 1!"), (this.state = "is-danger");
        return false;
      } else if (Validator.maxLength(val.trim(), 16)) {
        (this.errorMsg = "No longer than 16!"), (this.state = "is-danger");
        return false;
      } else if (Validator.validChar(val.trim())) {
        (this.errorMsg =
          'Only capital or lower-case letter, number and "_" are permitted!'),
          (this.state = "is-danger");
        return false;
      } else {
        this.state = "is-success";
        return true;
      }
    },

    handleChange(e) {
      let state = this.validate(e.target.value);
      this.$emit("change", e.target.value, state);
    }
  }
};
</script>

<style scoped>
.channel-wrapper {
  padding-bottom: 30px;
}
.channel-wrapper > span {
  height: 35px !important;
}

#channel {
  background: transparent;
  color: white;
  padding: 8px 50px;
  height: 35px;
}

#channel:focus:not(.is-success):not(.is-danger) {
  border-color: #00b6ed;
}

#channel::-webkit-input-placeholder {
  color: white;
  opacity: 0.5;
}

.channel-wrapper.has-icons-left .icon.is-left {
  left: 15px;
}

.validate-msg {
  width: 100%;
  position: absolute;
  bottom: -10px;
  color: #ff3860;
  text-align: center;
  font-size: 12px;
  height: 30px;
}

.validate-icon {
  position: absolute;
  right: -36px;
  top: 0px;
  color: red;
  font-size: 2rem;
  display: inline-flex;
  justify-content: center;
  align-items: center;
}

.validate-icon > .ag-icon-invalid {
  color: #d0021b;
}

.validate-icon > .ag-icon-valid {
  color: rgb(178, 206, 149);
}
</style>


