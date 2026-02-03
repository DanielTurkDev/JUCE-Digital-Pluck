%% MUT 403 - Digital Pluck Prototype
%% Name - [Daniel Turk]
%% Date - [1/28/2026]

%% Brief Description (5 points)
% Code produces an array containing a "plucked" sin wave that has an
% exponential decay 

%% Project Code



freq = 440; % Hz
amplitude = 1; % must be less than one
decay = 500; % ms
fs = 44100; % sample rate

makePluck(freq, amplitude, decay, fs)

function [y_n] = makePluck(frequency, amplitude, decay, fs)
    if frequency < 1
        error("Invalid frequency");
    end

    env_n = makeEnvelope(decay, fs)

    y_n = zeros(1, length(env_n));

    % Fill signal array 
    for sample_num = 1:length(env_n)
        t = (sample_num - 1) / fs;
        sin_val = amplitude * sin(2 * pi * frequency * t);
        y_n(sample_num) = sin_val * env_n(sample_num);
    end
    
    player = audioplayer(y_n, fs);
    playblocking(player);
end

%% Returns an array that exponentially decays to an arbitrarily small proportion
%% Within a given number of samples
function [env_n] = makeEnvelope(decay, fs)
    % Before starting, we should calculate our coefficent
    if decay < 1
        error("Invalid decay")
    end

    end_proportion = 0.01; % Target value at end of decay
    ms_per_second = 1000; % Used when converting decay in ms to a sample num

    end_pos = round(decay * fs / ms_per_second); % calculates sample at which we want the envelope to be arbitrarirly small value

    % We want y to be end_val when i = end_pos
    % formula: y = exp(-k * t)
    k = -log(end_proportion) / end_pos; % calculating coefficent 

    env_n = zeros(1,end_pos);

    for i = 1:end_pos
        % (i-1) makes first sample 1, exp(0) = 1
        env_n(i) = exp(-k * i); % Calculates envelope at each sample
    end
end


