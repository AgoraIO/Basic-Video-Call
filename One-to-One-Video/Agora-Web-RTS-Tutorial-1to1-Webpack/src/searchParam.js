export const parseFromSearch = () => {
  const searchStr = window.location.search
  const param = new URLSearchParams(searchStr)
  let map = {}
  for (let key of param.keys()) {
    map[key] = param.get(key)
  }
  return map
}

export const stringify = (obj) => {
  const str = Object.entries(obj).reduce((pre, cur) => {
    return pre + `${cur[0]}=${cur[1]}&`
  }, '?')
  return str.slice(0, -1)
}

export const setFormData = (obj) => {
  Object.keys(obj).map(key => {
    const targetDom = document.querySelector(`[name=${key}]`)
    if (targetDom) {
      targetDom.value = obj[key]
    }
  })
}

export const replaceSearchString = (searchStr) => {
  if (window.history.pushState) {
    const newURL = new URL(window.location.href);
    newURL.search = searchStr;
    window.history.pushState({ path: newURL.href }, "", newURL.href);
  } else {
    console.warn('`history.pushState` is not supported')
  }
}