import { useEffect, useState } from 'react';
import useMounted from './useMounted';
import AgoraRTC from '../utils/AgoraEnhancer';

const usePermission = () => {
  const [hasPermission, setHasPermission] = useState(false);
  const isMounted = useMounted();

  // request media permissions
  useEffect(() => {
    const tempAudioStream = AgoraRTC.createStream({ audio: true, video: false });
    const tempVideoStream = AgoraRTC.createStream({ audio: false, video: true });
    Promise
      .all([tempAudioStream.init(), tempVideoStream.init()])
      .then(_ => {
          return true;
      })
      .catch(error => {
        console.log(error)
        return false;
      })
      .then(has => {
        tempAudioStream.close();
        tempVideoStream.close();
        isMounted() && setHasPermission(has)
      })
  }, []);

  return hasPermission;
}

export default usePermission;
