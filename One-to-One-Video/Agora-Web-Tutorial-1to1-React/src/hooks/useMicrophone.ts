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

const useMicrophone = (client = fakeClient): MediaDeviceInfo[] => {
  const [microphoneList, setMicrophoneList] = useState<MediaDeviceInfo[]>([]);

  useEffect(() => {
    let mounted = true;

    const onChange = () => {
      if (!client) {
        return
      }
      client
        .getRecordingDevices()
        .then((microphones: MediaDeviceInfo[]) => {
          if (mounted) {
            setMicrophoneList(microphones);
          }
        })
        .catch(noop);
    };

    client && client.on('recording-device-changed', onChange);
    onChange();

    return () => {
      mounted = false;
      client && (client as IClientWithPromise & {gatewayClient: any}).gatewayClient.removeEventListener(
        'recordingDeviceChanged',
        onChange
      );
    };
  }, [client]);

  return microphoneList;
};

export default useMicrophone;
