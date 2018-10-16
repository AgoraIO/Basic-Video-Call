export const isSafari = () => {
  return /^((?!chrome|android).)*safari/i.test(navigator.userAgent);
};

export const isMobileSize = () => {
  if (window.innerWidth <= 800 && window.innerHeight <= 830) {
    return true;
  }
  return false;
};

export const isChrome = () => {
  return Boolean(window.chrome) && Boolean(window.chrome.webstore);
};

export const isFirefox = () => {
  // eslint-disable-next-line
  return typeof InstallTrigger !== 'undefined';
};
