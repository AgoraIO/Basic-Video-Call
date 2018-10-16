export default ({ width, height, minRatio, maxRatio, count }) => {
  let maxArea;
  let itemWidth;
  let itemHeight;
  let resultWidth;
  let resultHeight;
  // Let resultRows, resultCols
  for (let i = 1; i <= count; i++) {
    let rows = i;
    let cols = Math.ceil(count / i);

    itemWidth = Math.floor((width - 5) / cols);
    itemHeight = Math.floor((height - 5) / rows);

    let radio = itemHeight / itemWidth;
    // If radio > max or < min
    if (radio > maxRatio) {
      radio = maxRatio;
      itemHeight = radio * itemWidth;
    } else if (radio < minRatio) {
      radio = minRatio;
      itemWidth = itemHeight / radio;
    }

    let area = itemWidth * itemHeight * count;

    // If this width and height takes up the most space then we're going with that
    if (maxArea === undefined || area > maxArea) {
      maxArea = area;
      resultHeight = itemHeight;
      resultWidth = itemWidth;
      // ResultCols = cols
      // resultRows = rows
    }
  }

  return {
    height: resultHeight,
    width: resultWidth
  };
};
