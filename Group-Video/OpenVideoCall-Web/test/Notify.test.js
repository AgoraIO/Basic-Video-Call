import $ from 'jquery';
import Notify from '../src/utils/Notify.js';
it('Display a notification after use func Notify', () => {
  Notify.success('success: hello world');
  expect($('.notification-container')).toBeTruthy();
  expect($('.notification .is-success')).toBeTruthy();
});
