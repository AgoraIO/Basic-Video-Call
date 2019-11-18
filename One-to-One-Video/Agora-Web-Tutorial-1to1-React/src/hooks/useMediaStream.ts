import { useState, useEffect } from "react";

const useMediaStream = (client: any, filter?: (streamId: number) => boolean): any[] => {
  const [localStream, setLocalStream] = useState<any>(undefined);
  const [remoteStreamList, setRemoteStreamList] = useState<any[]>([]);

  useEffect(() => {
    let mounted = true;
    // add when subscribed
    const addRemote = (evt: any) => {
      if (!mounted) {
        return;
      }
      const { stream } = evt;
      setRemoteStreamList(streamList => [...streamList, stream]);
    };
    // remove stream
    const removeRemote = (evt: any) => {
      const { stream } = evt;
      if (stream) {
        const id = stream.getId();
        const index = remoteStreamList.findIndex(item => item.getId() === id);
        if (index !== -1) {
          setRemoteStreamList(streamList => {
            streamList.splice(index, 1);
            return streamList;
          });
        }
      }
    };
    // subscribe when added
    const doSub = (evt: any) => {
      if (!mounted) {
        return;
      }
      if (filter) {
        if (filter(evt.stream.getId())) {
          client.subscribe(evt.stream);
        }
      } else {
        client.subscribe(evt.stream);
      }
    };
    // add when published
    const addLocal = (evt: any) => {
      if (!mounted) {
        return;
      }
      const { stream } = evt;
      const stop = stream.stop;
      const close = stream.close;
      stream.close = (func => () => {
        func()     
        setLocalStream(undefined);   
      })(close);
      stream.stop = (func => () => {
        func()     
        setLocalStream(undefined);   
      })(stop);
      setLocalStream(stream)
    };

    if (client) {
      client.on("stream-published", addLocal);
      client.on("stream-added", doSub);
      client.on("stream-subscribed", addRemote);
      client.on("peer-leave", removeRemote);
      client.on("stream-removed", removeRemote);
    }

    return () => {
      mounted = false;
      if (client) {
        client.gatewayClient.removeEventListener("stream-published", addLocal);
        client.gatewayClient.removeEventListener("stream-added", doSub);
        client.gatewayClient.removeEventListener("stream-subscribed", addRemote);
        client.gatewayClient.removeEventListener("peer-leave", removeRemote);
        client.gatewayClient.removeEventListener("stream-removed", removeRemote);
      }
    };
  }, [client, filter]);

  return [localStream, remoteStreamList, [localStream].concat(remoteStreamList)];
};

export default useMediaStream;
