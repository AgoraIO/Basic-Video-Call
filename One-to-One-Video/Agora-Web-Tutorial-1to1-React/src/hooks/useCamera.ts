import { useState, useEffect } from 'react';
import AgoraRTC from '../utils/AgoraEnhancer';
import { IClientWithPromise } from 'agoran-awe/types/promisify';

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
    
    client && client.on('camera-changed', onChange);
    
    onChange();

    return () => {
      mounted = false;
      client && (client as IClientWithPromise & {gatewayClient: any}).gatewayClient.removeEventListener('cameraChanged', onChange);
    };
  }, [client]);

  return cameraList;
};

export default useCamera;
