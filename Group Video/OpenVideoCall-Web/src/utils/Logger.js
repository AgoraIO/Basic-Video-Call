const log = (prefix, color, info) => {
  console.log(`%c ${prefix}: ${info}`, `color: ${color}`);
};

const logger = {
  init(prefix, color) {
    return info => log(prefix, color, info);
  }
};

export { logger, log };
