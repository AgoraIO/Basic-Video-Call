import calcSize from "./calcSize";
import "./render.css";

export default {
  init(elementId, minRatio, maxRatio) {
    this.MAX_RATIO = maxRatio;
    this.MIN_RATIO = minRatio;
    this.canvas = document.querySelector(`#${elementId}`);
  },

  _checkRatio(width, height) {
    let ratio = height / width;
    if (ratio > this.MAX_RATIO || ratio < this.MIN_RATIO) {
      return false;
    }
    return true;
  },

  customRender(streamList, mode, mainId) {
    // Reinit canvas style first
    this.canvas.classList.remove("container__flex");
    this.canvas.classList.add("container__grid");

    // Get no
    let no = streamList.length;
    mainId =
      mainId ||
      (streamList[streamList.length - 1] &&
        streamList[streamList.length - 1].getId());

    // We should consider no, isMobileSize, currentMode
    if ((no > 4 && mode === 1) || no > 8) {
      mode = 0;
      console.log("Automatically switch to tile mode...");
      this.rendererFactory(streamList, mode, mainId);
    } else {
      this.rendererFactory(streamList, mode, mainId);
    }
  },

  rendererFactory(streamList, mode, mainId) {
    if (mode === 0) {
      this.tileRenderer(streamList);
    } else if (mode === 1) {
      this.pipRenderer(streamList, mainId);
    } else if (mode === 2) {
      this.sharingRenderer(streamList, mainId);
    } else {
      throw Error("Wrong mode for renderer");
    }
  },

  updateVideoItem(stream, style, fit = false) {
    let id = stream.getId();
    let dom = document.querySelector("#video-item-" + id);
    if (!dom) {
      dom = document.createElement("section");
      let box = document.createElement("div");
      dom.setAttribute("id", "video-item-" + id);
      dom.setAttribute("class", "video-item");
      box.setAttribute("class", "video-item-box");
      dom.appendChild(box);
      this.canvas.appendChild(dom);
      stream.play("video-item-" + id);
    }
    if (fit) {
      dom.classList.add("window__fit");
    } else {
      dom.classList.remove("window__fit");
    }
    dom.setAttribute("style", style);
  },

  enterFullScreen() {
    this.canvas.classList.add("fullscreen");
  },

  exitFullScreen() {
    this.canvas.classList.remove("fullscreen");
  },

  /**
   * @description Tile mode renderer. Recommended for 1-N people.
   */
  tileRenderer(streamList) {
    let { width, height } = calcSize({
      width: this.canvas.clientWidth,
      height: this.canvas.clientHeight,
      minRatio: this.MIN_RATIO,
      maxRatio: this.MAX_RATIO,
      count: streamList.length
    });
    // Use flex box container
    this.canvas.classList.remove("container__grid");
    this.canvas.classList.add("container__flex");
    for (let stream of streamList) {
      this.updateVideoItem(stream, `width: ${width}px; height: ${height}px;`);
    }
  },

  /**
   * @description PIP mode renderer. Recommended for 1-4 people.
   */
  pipRenderer(streamList, mainId) {
    let no = streamList.length;
    if (no > 4) {
      throw Error("PIP mode only suitable for less than 4 stream");
    }

    // Check ratio before using pip ratio
    if (
      !this._checkRatio(
        (this.canvas.clientWidth * 4) / 24,
        (this.canvas.clientHeight * 3) / 12
      ) ||
      !this._checkRatio(
        (this.canvas.clientWidth * 12) / 24,
        (this.canvas.clientHeight * 12) / 12
      )
    ) {
      return this.tileRenderer(streamList);
    }

    // Now you can render in pip mode
    for (let index = 0, count = 0; index < no; index++) {
      let stream = streamList[index];
      if (stream.getId() === mainId) {
        // Main window
        this.updateVideoItem(stream, `grid-area: span 12/span 24/13/25`);
      } else {
        // Sub window
        this.updateVideoItem(
          stream,
          `grid-area: span 3/span 4/${4 + 3 * count}/25;
                    z-index:1;width:calc(100% - 20px);height:calc(100% - 20px)`
        );
        count++;
      }
    }
  },

  /**
   * @description Screen sharing mode renderer. Recommended for 1-7 people + 1 sharing stream.
   */
  sharingRenderer(streamList, mainId) {
    let no = streamList.length;
    if (no > 8) {
      throw Error("Screen Sharing Mode only suitable for less than 8 stream");
    }

    // Check ratio before using screen sharing ratio unless there is only one stream
    if (
      !this._checkRatio(
        (this.canvas.clientWidth * 4) / 24,
        (this.canvas.clientHeight * 4) / 12
      ) &&
      streamList.length !== 1
    ) {
      // Hide other streams
      let mainStreamIndex = streamList.findIndex(function(element) {
        return element.getId() === mainId;
      });
      if (mainStreamIndex === -1) {
        throw Error("Cannot find stream by given mainId!");
      }
      // Only render main stream(sharing stream)
      for (let i = 0; i < no; i++) {
        if (i !== mainStreamIndex) {
          this.updateVideoItem(streamList[i], "display: none");
        }
      }
      return this.sharingRenderer([streamList[mainStreamIndex]], mainId);
    }

    // Copy a temp streamList
    let tempStreamList = [...streamList];
    // Now you can use screen sharing mode
    if (no === 8) {
      // When there are 7 people with 1 sharing stream, hide audio stream or local stream
      // try to find first audio stream and splice it, if not splice local stream
      let shouldRemoveStreamIndex = tempStreamList.findIndex(function(element) {
        return element.hasAudio() && !element.hasVideo();
      });
      if (shouldRemoveStreamIndex === -1) {
        shouldRemoveStreamIndex = 7;
      }
      let shouldRemoveStream = tempStreamList[shouldRemoveStreamIndex];
      this.updateVideoItem(shouldRemoveStream, "display: none");
      tempStreamList.splice(shouldRemoveStreamIndex, 1);
    }

    for (let stream of tempStreamList) {
      if (stream.getId() === mainId) {
        // When there were less than 5 people
        // sharing stream will take more place
        if (no === 1) {
          this.updateVideoItem(
            stream,
            `grid-area: span 12/span 24/13/25;`,
            true
          );
        } else if (no < 5) {
          this.updateVideoItem(
            stream,
            `grid-area: span 12/span 20/13/25;`,
            true
          );
        } else {
          this.updateVideoItem(
            stream,
            `grid-area: span 12/span 16/13/21;`,
            true
          );
        }
      } else {
        // Normal stream
        this.updateVideoItem(stream, `grid-area: span 4/span 4`);
      }
    }
  }
};
