import React, { useReducer, useState } from "react";
import { makeStyles } from "@material-ui/core/styles";
import Grid from "@material-ui/core/Grid";
import Typography from "@material-ui/core/Typography";
import Container from "@material-ui/core/Container";
import AppBar from "@material-ui/core/AppBar";
import Toolbar from "@material-ui/core/Toolbar";
import Card from "@material-ui/core/Card";
import CardActions from "@material-ui/core/CardActions";
import CardContent from "@material-ui/core/CardContent";
import Button from "@material-ui/core/Button";
import TextField from "@material-ui/core/TextField";
import MenuItem from "@material-ui/core/MenuItem";
import StreamPlayer from 'agora-stream-player'

import { useCamera, useMicrophone, useMediaStream } from "./hooks";
import AgoraRTC from "./utils/AgoraEnhancer";

const useStyles = makeStyles(theme => ({
  root: {
    flexGrow: 1,
    padding: 12
  },
  title: {
    fontSize: 32,
    textAlign: "center"
  },
  divider: {
    marginBottom: "20px"
  },
  buttonContainer: {
    display: "flex",
    justifyContent: "space-around"
  },
  buttonItem: {
    width: "38.2%"
  }
}));

const agoraClient = AgoraRTC.createClient({ mode: "rtc", codec: "h264" });

const defaultState = {
  appId: "",
  channel: "",
  uid: "",
  token: undefined,
  cameraId: "",
  microphoneId: ""
};

const reducer = (
  state: typeof defaultState,
  action: { type: string; [propName: string]: any }
) => {
  switch (action.type) {
    default:
      return state;
    case "setAppId":
      return {
        ...state,
        appId: action.value
      };
    case "setChannel":
      return {
        ...state,
        channel: action.value
      };
    case "setUid":
      return {
        ...state,
        uid: action.value
      };
    case "setToken":
      return {
        ...state,
        token: action.value
      };
    case "setCamera":
      return {
        ...state,
        cameraId: action.value
      };
    case "setMicrophone":
      return {
        ...state,
        microphoneId: action.value
      };
  }
};

export default function App() {
  const classes = useStyles();
  const [isJoined, setisJoined] = useState(false);
  const [isPublished, setIsPublished] = useState(false);
  const [state, dispatch] = useReducer(reducer, defaultState);
  const cameraList = useCamera(agoraClient);
  const microphoneList = useMicrophone(agoraClient);
  let [localStream, remoteStreamList, streamList] = useMediaStream(agoraClient);

  const update = (actionType: string) => (
    e: React.ChangeEvent<HTMLInputElement>
  ) => {
    return dispatch({ type: actionType, value: e.target.value });
  };

  const join = async () => {
    const uid = isNaN(Number(state.uid)) ? undefined : Number(state.uid);
    await agoraClient.init(state.appId);
    await agoraClient.join(state.token, state.channel, uid);
    const stream = AgoraRTC.createStream({
      streamID: uid || 12345,
      video: true,
      audio: true,
      screen: false
    });
    // stream.setVideoProfile('480p_4')
    await stream.init();
    await agoraClient.publish(stream);
    setIsPublished(true);
    setisJoined(true);
  };

  const publish = async () => {
    if (localStream) {
      await agoraClient.publish(localStream);
      setIsPublished(true);
    }
  };

  const leave = async () => {
    if (localStream) {
      localStream.close();
      agoraClient.unpublish(localStream);
    }
    await agoraClient.leave();
    setIsPublished(false);
    setisJoined(false);
  };

  const unpublish = () => {
    if (localStream) {
      agoraClient.unpublish(localStream);
      setIsPublished(false);
    }
  };

  const JoinLeaveBtn = () => {
    return (
      <Button
        className={classes.buttonItem}
        color={isJoined ? "secondary" : "primary"}
        onClick={isJoined ? leave : join}
        variant="contained"
      >
        {isJoined ? "Leave" : "Join"}
      </Button>
    );
  };

  const PubUnpubBtn = () => {
    return (
      <Button
        className={classes.buttonItem}
        color={isPublished ? "secondary" : "default"}
        onClick={isPublished ? unpublish : publish}
        variant="contained"
        disabled={!isJoined}
      >
        {isPublished ? "Unpublish" : "Publish"}
      </Button>
    );
  };

  return (
    <React.Fragment>
      <AppBar color="default">
        <Toolbar>
          <Typography variant="h6">Basic Communication</Typography>
        </Toolbar>
      </AppBar>
      <Toolbar className={classes.divider} />
      <Container>
        <Grid container spacing={3}>
          {/* form */}
          <Grid item xs={12} md={4}>
            <Card>
              <CardContent>
                <form noValidate autoComplete="off">
                  <TextField
                    required
                    value={state.appId}
                    onChange={update("setAppId")}
                    id="appId"
                    label="App ID"
                    fullWidth
                    margin="normal"
                  />
                  <TextField
                    required
                    value={state.channel}
                    onChange={update("setChannel")}
                    id="channel"
                    label="Channel"
                    fullWidth
                    margin="normal"
                  />
                  <TextField
                    value={state.uid}
                    id="uid"
                    onChange={update("setUid")}
                    label="UID"
                    fullWidth
                    margin="normal"
                  />
                  <TextField
                    value={state.token}
                    onChange={update("setToken")}
                    id="token"
                    label="Token"
                    fullWidth
                    margin="normal"
                  />
                  <TextField
                    id="cameraId"
                    value={state.cameraId}
                    onChange={update("setCamera")}
                    select
                    label="Camera"
                    helperText="Please select your camera"
                    fullWidth
                    margin="normal"
                  >
                    {cameraList.map(item => (
                      <MenuItem key={item.deviceId} value={item.deviceId}>
                        {item.label}
                      </MenuItem>
                    ))}
                  </TextField>
                  <TextField
                    id="microphoneId"
                    value={state.microphoneId}
                    onChange={update("setMicrophone")}
                    select
                    label="Microphone"
                    helperText="Please select your microphone"
                    fullWidth
                    margin="normal"
                  >
                    {microphoneList.map(item => (
                      <MenuItem key={item.deviceId} value={item.deviceId}>
                        {item.label}
                      </MenuItem>
                    ))}
                  </TextField>
                </form>
              </CardContent>
              <CardActions className={classes.buttonContainer}>
                <JoinLeaveBtn />
                <PubUnpubBtn />
              </CardActions>
            </Card>
          </Grid>
          
          {/* display area */}
          <Grid item xs={12} md={8}>
            {(localStream) && <StreamPlayer stream={localStream} fit="contain" label="local" />}
            {remoteStreamList.map((stream: any) => (
              <StreamPlayer stream={stream} fit="contain" label={stream.getId()} />
            ))}
          </Grid>
        </Grid>
      </Container>
    </React.Fragment>
  );
}
