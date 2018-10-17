export default (() => {
  // Object.entries
  if (!Object.entries)
    Object.entries = function(obj) {
      let ownProps = Object.keys(obj);
      let i = ownProps.length;
      let resArray = new Array(i); // Preallocate the Array
      while (i--) resArray[i] = [ownProps[i], obj[ownProps[i]]];

      return resArray;
    };
})();
