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
import ExpansionPanel from "@material-ui/core/ExpansionPanel";
import ExpansionPanelSummary from "@material-ui/core/ExpansionPanelSummary";
import ExpansionPanelDetails from "@material-ui/core/ExpansionPanelDetails";
import ExpandMoreIcon from "@material-ui/icons/ExpandMore";
import FormControlLabel from "@material-ui/core/FormControlLabel";
import FormControl from "@material-ui/core/FormControl";
import FormLabel from "@material-ui/core/FormLabel";
import Radio from "@material-ui/core/Radio";
import RadioGroup from "@material-ui/core/RadioGroup";
import { createMuiTheme } from "@material-ui/core/styles";
import { ThemeProvider } from "@material-ui/styles";
import StreamPlayer from "agora-stream-player";
import { SnackbarProvider, useSnackbar } from "notistack";

import { useCamera, useMicrophone, useMediaStream } from "./hooks";
import AgoraRTC from "./utils/AgoraEnhancer";

const useStyles = makeStyles(theme => ({
  root: {
    flexGrow: 1,
    padding: 12
  },
  title: {
    fontWeight: 400
  },
  divider: {
    marginBottom: "32px"
  },
  buttonContainer: {
    display: "flex",
    justifyContent: "space-around"
  },
  buttonItem: {
    width: "38.2%"
  },
  advanceSettings: {
    marginTop: 16
  }
}));

const defaultState = {
  appId: "",
  channel: "",
  uid: "",
  token: undefined,
  cameraId: "",
  microphoneId: "",
  mode: "rtc",
  codec: "h264"
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
    case "setMode":
      return {
        ...state,
        mode: action.value
      };
    case "setCodec":
      return {
        ...state,
        codec: action.value
      };
  }
};

function App() {
  const classes = useStyles();
  const [isJoined, setisJoined] = useState(false);
  const [isPublished, setIsPublished] = useState(false);
  const [isLoading, setIsLoading] = useState(false);
  const [state, dispatch] = useReducer(reducer, defaultState);
  const [agoraClient, setClient] = useState<any>(undefined)
  // const agoraClient = AgoraRTC.createClient({ mode: state.mode, codec: state.codec });
  const cameraList = useCamera();
  const microphoneList = useMicrophone();
  let [localStream, remoteStreamList, streamList] = useMediaStream(agoraClient);
  const { enqueueSnackbar } = useSnackbar();

  const update = (actionType: string) => (e: React.ChangeEvent<unknown>) => {
    return dispatch({
      type: actionType,
      value: (e.target as HTMLInputElement).value
    });
  };

  const join = async () => {
    const client = AgoraRTC.createClient({ mode: state.mode, codec: state.codec })
    setClient(client)
    setIsLoading(true);
    try {
      const uid = isNaN(Number(state.uid)) ? null : Number(state.uid);
      await client.init(state.appId);
      await client.join(state.token, state.channel, uid);
      const stream = AgoraRTC.createStream({
        streamID: uid || 12345,
        video: true,
        audio: true,
        screen: false
      });
      // stream.setVideoProfile('480p_4')
      await stream.init();
      await client.publish(stream);
      setIsPublished(true);
      setisJoined(true);
      enqueueSnackbar(`Joined channel ${state.channel}`, { variant: "info" });
    } catch (err) {
      enqueueSnackbar(`Failed to join, ${err}`, { variant: "error" });
    } finally {
      setIsLoading(false);
    }
  };

  const publish = async () => {
    setIsLoading(true);
    try {
      if (localStream) {
        await agoraClient.publish(localStream);
        setIsPublished(true);
      }
      enqueueSnackbar("Stream published", { variant: "info" });
    } catch (err) {
      enqueueSnackbar(`Failed to publish, ${err}`, { variant: "error" });
    } finally {
      setIsLoading(false);
    }
  };

  const leave = async () => {
    setIsLoading(true);
    try {
      if (localStream) {
        localStream.close();
        agoraClient.unpublish(localStream);
      }
      await agoraClient.leave();
      setIsPublished(false);
      setisJoined(false);
      enqueueSnackbar("Left channel", { variant: "info" });
    } catch (err) {
      enqueueSnackbar(`Failed to leave, ${err}`, { variant: "error" });
    } finally {
      setIsLoading(false);
    }
  };

  const unpublish = () => {
    if (localStream) {
      agoraClient.unpublish(localStream);
      setIsPublished(false);
      enqueueSnackbar("Stream unpublished", { variant: "info" });
    }
  };

  const JoinLeaveBtn = () => {
    return (
      <Button
        className={classes.buttonItem}
        color={isJoined ? "secondary" : "primary"}
        onClick={isJoined ? leave : join}
        variant="contained"
        disabled={isLoading}
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
        disabled={!isJoined || isLoading}
      >
        {isPublished ? "Unpublish" : "Publish"}
      </Button>
    );
  };

  return (
    <React.Fragment>
      <AppBar color="primary">
        <Toolbar>
          <Typography className={classes.title} variant="h6">
            Basic Communication
          </Typography>
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
                    value={state.token}
                    onChange={update("setToken")}
                    id="token"
                    label="Token"
                    fullWidth
                    margin="normal"
                  />
                </form>
              </CardContent>
              <CardActions className={classes.buttonContainer}>
                <JoinLeaveBtn />
                <PubUnpubBtn />
              </CardActions>
            </Card>

            {/* advanced settings */}
            <ThemeProvider
              theme={createMuiTheme({
                palette: {
                  type: "dark"
                }
              })}
            >
              <ExpansionPanel className={classes.advanceSettings}>
                <ExpansionPanelSummary expandIcon={<ExpandMoreIcon />}>
                  <Typography>Advanced Settings</Typography>
                </ExpansionPanelSummary>
                <ExpansionPanelDetails>
                  <form noValidate autoComplete="off">
                    <TextField
                      value={state.uid}
                      id="uid"
                      onChange={update("setUid")}
                      label="UID"
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

                    <FormControl fullWidth component="fieldset" margin="normal">
                      <FormLabel>Mode</FormLabel>
                      <RadioGroup
                        row
                        value={state.mode}
                        onChange={update("setMode")}
                      >
                        <FormControlLabel
                          value="live"
                          control={<Radio color="primary" />}
                          label="live"
                        />
                        <FormControlLabel
                          value="rtc"
                          control={<Radio color="primary" />}
                          label="rtc"
                        />
                      </RadioGroup>
                    </FormControl>

                    <FormControl fullWidth component="fieldset" margin="normal">
                      <FormLabel>Codec</FormLabel>
                      <RadioGroup
                        row
                        value={state.codec}
                        onChange={update("setCodec")}
                      >
                        <FormControlLabel
                          value="vp8"
                          control={<Radio color="primary" />}
                          label="vp8"
                        />
                        <FormControlLabel
                          value="h264"
                          control={<Radio color="primary" />}
                          label="h264"
                        />
                      </RadioGroup>
                    </FormControl>
                  </form>
                </ExpansionPanelDetails>
              </ExpansionPanel>
            </ThemeProvider>
          </Grid>

          {/* display area */}
          <Grid item xs={12} md={8}>
            {localStream && (
              <StreamPlayer stream={localStream} fit="contain" label="local" />
            )}
            {remoteStreamList.map((stream: any) => (
              <StreamPlayer
                key={stream.getId()}
                stream={stream}
                fit="contain"
                label={stream.getId()}
              />
            ))}
          </Grid>
        </Grid>
      </Container>
    </React.Fragment>
  );
}

export default function AppWithNotification() {
  return (
    <SnackbarProvider
      anchorOrigin={{ vertical: "top", horizontal: "right" }}
      autoHideDuration={2500}
      maxSnack={5}
    >
      <App />
    </SnackbarProvider>
  );
}
