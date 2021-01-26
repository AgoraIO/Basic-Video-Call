import { useEffect, useCallback, useRef } from 'react';

const useMounted = (): (() => boolean) => {
  const ref = useRef<boolean>(false);
  useEffect(() => {
    ref.current = true;
    return () => {
      ref.current = false;
    };
  }, []);
  return useCallback(() => ref.current, []);
};

export default useMounted;
