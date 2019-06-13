import AgoraRTC from "agora-rtc-sdk";
import enhanceAgoraRTC from "agoran-awe";
// promisify class Client & Stream
const enhancedAgoraRTC = enhanceAgoraRTC(AgoraRTC)
export default enhancedAgoraRTC