% Gap distances (mm)
gaps = [0.0762, 0.2762, 0.4762];

% Pressure values (mmHg)
pressures = [0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50];

% Contact areas (mm^2) for each gap
areas = [
    0, 264.203, 388.511, 449.261, 477.265, 490.003, 512.31, 514.521, 521.351, 520.984, 522.076;
    0, 234.982, 366.712, 429.751, 463.586, 481.986, 506.351, 512.361, 519.128, 520.743, 521.743;
    0, 222.751, 355.683, 421.987, 460.412, 480.152, 507.987, 511.562, 512.512, 518.142, 519.417
];

% Total possible contact area (mm^2)
total_area = 1135.024753178212;

% Normalize to fraction of total area
areas_norm = areas / total_area;

% Plot
figure;
hold on;
colors = lines(length(gaps));

for i = 1:length(gaps)
    plot(pressures, areas_norm(i,:), '-o', 'Color', colors(i,:), ...
        'MarkerFaceColor', colors(i,:), 'LineWidth', 3.5, ...
        'DisplayName', sprintf('Gap = %.4f mm', gaps(i)));
end

title('Pressure vs Normalized Contact Area', 'FontSize', 42, 'FontWeight', 'bold');
xlabel('Pressure (mmHg)', 'FontSize', 38, 'FontWeight', 'bold');
ylabel('Normalized Contact Area (fraction of total)', 'FontSize', 38, 'FontWeight', 'bold');
yticks(0:0.1:1); % for readability
lgd = legend('Location', 'best');
title(lgd, 'Gap Distance');
grid on;
set(gca, 'FontSize', 38, 'LineWidth', 3.2);
hold off;
