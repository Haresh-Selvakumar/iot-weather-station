% Controlled cloud tests, not DHT22 sensor samples.
% Run one write at a time, at least 20 seconds apart.
% Set private keys in your working copy only.
channelID = 3499317;
writeKey = 'YOUR_CHANNEL_WRITE_KEY';
readKey = 'YOUR_CHANNEL_READ_KEY';

% High-temperature test:
thingSpeakWrite(channelID, [40 60 1], 'Fields', [1 2 3], 'WriteKey', writeKey);
pause(20);
% Return to normal to rearm the first-occurrence React rule:
thingSpeakWrite(channelID, [28 60 0], 'Fields', [1 2 3], 'WriteKey', writeKey);
pause(20);
% High-humidity test:
thingSpeakWrite(channelID, [28 85 1], 'Fields', [1 2 3], 'WriteKey', writeKey);

% Readback (run separately after writes):
v = thingSpeakRead(channelID, 'Fields', [1 2 3], 'NumPoints', 3, 'ReadKey', readKey);
disp(v);
% For ThingSpeak online analysis, run the writes as separate jobs;
% do not rely on long pauses within one execution.
% Observe the Alerts API quota before repeating these tests.
