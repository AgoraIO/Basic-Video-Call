import $ from 'jquery';

const ButtonControl = (function() {
  let handler = (param, callback) => {
    if (param instanceof Array) {
      param.map(item => {
        return callback(item);
      });
    } else {
      callback(param);
    }
  };
  let hide = ids => {
    handler(ids, function(id) {
      $(id).hide();
    });
  };
  let show = ids => {
    handler(ids, function(id) {
      $(id).show();
    });
  };
  let toggle = ids => {
    handler(ids, function(id) {
      $(id).toggle();
    });
  };
  let enable = ids => {
    handler(ids, function(id) {
      $(id).removeClass('disabled');
    });
  };
  let disable = ids => {
    handler(ids, function(id) {
      $(id).addClass('disabled');
    });
  };

  return {
    hide,
    show,
    toggle,
    enable,
    disable
  };
})();

export default ButtonControl;
