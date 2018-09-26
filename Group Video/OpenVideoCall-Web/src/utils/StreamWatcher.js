const StreamWatcher = callback => {
  const arrayProto = Array.prototype;
  const arrayMethods = Object.create(arrayProto);
  const newArrProto = [];
  const methodsArr = ['push', 'pop', 'shift', 'unshift', 'splice', 'sort', 'reverse'];
  methodsArr.forEach(method => {
    let original = arrayMethods[method];
    newArrProto[method] = function() {
      let result = original.apply(this, arguments);
      callback();
      return result;
    };
  });

  return newArrProto;
};

// Use 'streamList.__proto__ = StreamWatcher' to add this watcher
export default StreamWatcher;
