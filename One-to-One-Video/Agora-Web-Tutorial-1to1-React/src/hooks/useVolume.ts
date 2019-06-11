import { useState, useEffect } from 'react';

const useVolume = (stream: any, interval = 50): number => {
  const [volume, setVolume] = useState(0);

  useEffect(() => {
    let mounted = true;

    const timer = setInterval(() => {
      if (mounted) {
        if (stream && stream.getAudioLevel) {
          setVolume(stream.getAudioLevel());
        } else {
          setVolume(0);
        }
      }
    }, interval);

    return () => {
      mounted = false;
      clearInterval(timer);
    };
  }, [stream]);

  return volume;
};

export default useVolume;
