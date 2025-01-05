function wavelength__m = getCarrierPhaseWavelength(f1, f2)

    % g = gcd(f1,f2);
    % iA = f1/g;
    % iB = -f2/g;

    lambda_1 = const.c__mDs/f1;
    lambda_2 = const.c__mDs/f2;

    % wavelength__m = (lambda_1*lambda_2) / ((iA * lambda_2) + (iB * lambda_1));
    wavelength__m = (lambda_1*lambda_2) / (lambda_2 + lambda_1); %narrow lane
end