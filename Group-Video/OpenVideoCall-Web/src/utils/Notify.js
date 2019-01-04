// Notification-related classes are defined by bulma.css temporarily

/**
 * add notification container to body
 */
import $ from 'jquery';
const notificationInit = () => {
  let container = `<div class="notification-container" 
                    style="z-index: 12;position: absolute;
                    width: 38.2%;max-width: 450px; 
                    min-width: 300px;left: 0;
                    word-break:break-all;
                    bottom: 0;"></div>`;
  $('body').append(container);
};

/**
 * Return either primary/link/info/success/warning/danger kind of notify
 */
const Notify = (() => {
  // Singleton for notification container
  let container = $('.notification-container');
  if (!container.length) {
    notificationInit();
    container = $('.notification-container');
  }

  let notifyFactory = (type, msg, secs) => {
    let id = new Date().getTime();
    let notification = `<div id="notify-${id}" class="notification is-${type}">
                                <button class="delete"></button>
                                ${msg}
                            </div>`;
    container.append(notification);
    $('#notify-' + id + ' .delete').on('click', function() {
      $('#notify-' + id).remove();
    });
    setTimeout(function() {
      $('#notify-' + id).remove();
    }, secs);
  };

  return {
    primary(msg, secs) {
      notifyFactory('primary', msg, secs);
    },
    link(msg, secs) {
      notifyFactory('link', msg, secs);
    },
    info(msg, secs) {
      notifyFactory('info', msg, secs);
    },
    success(msg, secs) {
      notifyFactory('success', msg, secs);
    },
    warning(msg, secs) {
      notifyFactory('warning', msg, secs);
    },
    danger(msg, secs) {
      notifyFactory('danger', msg, secs);
    }
  };
})();

export default Notify;
