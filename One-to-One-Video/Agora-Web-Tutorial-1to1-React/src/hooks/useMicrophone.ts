import { useState, useEffect } from 'react';

const noop = () => {};

interface MediaDeviceInfo {
  label: string;
  deviceId: string;
}

const useMicrophone = (client: any): MediaDeviceInfo[] => {
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

    client && client.on('recordingDeviceChanged', onChange);
    onChange();

    return () => {
      mounted = false;
      client && client.gatewayClient.removeEventListener(
        'recordingDeviceChanged',
        onChange
      );
    };
  }, []);

  return microphoneList;
};

export default useMicrophone;
