import Vue from 'vue'
import Router from 'vue-router'
import Home from './Home'
import Meeting from './Meeting'

Vue.use(Router)

export default new Router({
  routes: [
    {
      path: '/',
      name: 'Home',
      component: Home
    },
    {
      path: '/meeting',
      name: 'Meeting',
      component: Meeting
    }
  ]
})
