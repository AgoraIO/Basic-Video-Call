import { useState, useEffect } from 'react';
import AgoraRTC from '../utils/AgoraEnhancer';

const fakeClient = AgoraRTC.createClient({
  mode: 'live',
  codec: 'vp8'
})

const noop = () => {};

interface MediaDeviceInfo {
  label: string;
  deviceId: string;
}

const useCamera = (client = fakeClient): MediaDeviceInfo[] => {
  const [cameraList, setCameraList] = useState<MediaDeviceInfo[]>([]);

  useEffect(() => {
    let mounted = true;

    const onChange = () => {
      if (!client) {
        return;
      }
      client
        .getCameras()
        .then((cameras: MediaDeviceInfo[]) => {
          if (mounted) {
            setCameraList(cameras);
          }
        })
        .catch(noop);
    };
    
    client && client.on('cameraChanged', onChange);
    
    onChange();

    return () => {
      mounted = false;
      client && client.gatewayClient.removeEventListener('cameraChanged', onChange);
    };
  }, [client]);

  return cameraList;
};

export default useCamera;
