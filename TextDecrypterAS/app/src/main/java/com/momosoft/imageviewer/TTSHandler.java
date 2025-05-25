package com.momosoft.imageviewer;

import android.app.Activity;
import android.content.Intent;
import android.speech.tts.TextToSpeech;
import android.speech.tts.UtteranceProgressListener;

import java.util.Locale;

public class TTSHandler {
    public static final int REQUEST_CODE_CHECK_TTS = 2;

    private Activity m_Activity;
    private TextToSpeech m_TTS;
    private String[] m_MessageChunks;
    private int m_MsgChunkIndex = 0;

    TTSHandler(Activity activity) {
        m_Activity = activity;
    }

    public void checkIfInstalled() {
        Intent CheckTTSIntent = new Intent(TextToSpeech.Engine.ACTION_CHECK_TTS_DATA);
        m_Activity.startActivityForResult(CheckTTSIntent, REQUEST_CODE_CHECK_TTS);
    }

    public void setText(String text) {
        m_MessageChunks = text.split("(?<=[.!?]|:\\)|:D|;\\)|:P|XD)\\s+");
    }

    public void HandleTTSCheck(int resultCode) {
        if (resultCode == TextToSpeech.Engine.CHECK_VOICE_DATA_PASS) {
            m_Activity.findViewById(R.id.button).setEnabled(false);
            m_TTS = new TextToSpeech(m_Activity, new TextToSpeech.OnInitListener() {
                @Override
                public void onInit(int status) {
                    if (status == TextToSpeech.SUCCESS) {
                        int result = m_TTS.setLanguage(new Locale("hu", "HU"));

                        if (result == TextToSpeech.LANG_MISSING_DATA || result == TextToSpeech.LANG_NOT_SUPPORTED) {
                            //handle
                        }
                        else {
                            m_Activity.findViewById(R.id.button).setEnabled(true);
                        }
                    } else {
                        //handle
                    }
                }
            });


            m_TTS.setOnUtteranceProgressListener(new UtteranceProgressListener() {
                @Override
                public void onStart(String utteranceId) {
                }

                @Override
                public void onDone(String utteranceId) {
                    if (m_MsgChunkIndex == m_MessageChunks.length - 1) {
                        m_TTS.stop();
                        m_Activity.runOnUiThread(() -> {
                            m_Activity.findViewById(R.id.button).setEnabled(true);
                            m_Activity.findViewById(R.id.button2).setEnabled(true);
                        });

                    }
                    else {
                        playMessageChunk(++m_MsgChunkIndex);
                    }
                }

                @Override
                public void onError(String utteranceId) {
                    //handle
                }
            });

        }
        else {
            //messagebox, install tts
            Intent installIntent = new Intent(TextToSpeech.Engine.ACTION_INSTALL_TTS_DATA);
            m_Activity.startActivity(installIntent);
        }
    }

    private void playMessageChunk(int chunkIdx)
    {
        m_TTS.speak(m_MessageChunks[chunkIdx], TextToSpeech.QUEUE_FLUSH, null, "chunkid");
    }

    public void playMessage() {
        m_MsgChunkIndex = 0;
        playMessageChunk(0);
    }
}
