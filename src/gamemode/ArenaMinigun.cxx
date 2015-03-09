/*
	Copyright (c) 2014 - 2015 Rafa� "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa� Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file

	________________________________________________________________	
	Notes

	________________________________________________________________
	Dependencies

	________________________________________________________________
	Project Contributors
		Tom 'IceCube' Hewlett
		Mariusz 'C2A1' K.
		Mateusz 'eider' Cicho�
*/
#include "GameMode.hxx"

using namespace Zabawy;

namespace Minigun
{
	class Arena : public Base
	{
	public:
		std::vector<SObject> ArenaObjects;

		std::vector<SSpawnPoint> ArenaSpawns;

		Arena() :
			Base(L_games_ArenaMinigun_name, "/mng"),
			ArenaSpawns({
				{ 2671.1790, 2716.7612, 12.8203, 74.1882 },
				{ 2651.5635, 2731.7983, 12.8203, 0.5541 },
				{ 2670.7302, 2756.8538, 12.8203, 87.0350 },
				{ 2650.3022, 2698.3823, 21.3222, 254.4291 },
				{ 2642.4575, 2704.2356, 27.8222, 15.5943 },
				{ 2622.6626, 2716.1550, 38.5386, 31.8878 },
				{ 2606.3645, 2726.4631, 25.8222, 5.4226 },
				{ 2603.0430, 2784.7346, 18.5722, 0.2642 },
				{ 2585.9204, 2807.6433, 29.8203, 0.7460 },
				{ 2544.7915, 2837.5105, 29.8203, 269.4201 },
				{ 2626.8599, 2840.4614, 12.8203, 82.9850 },
				{ 2592.5569, 2848.5396, 12.8203, 179.8059 },
				{ 2579.0447, 2806.6890, 12.8203, 3.0843 },
				{ 2562.7014, 2848.0012, 21.9922, 276.4585 },
				{ 2613.7400, 2805.8875, 21.9922, 88.1668 },
				{ 2501.3879, 2843.5935, 12.8203, 270.8419 },
				{ 2501.1509, 2807.7527, 16.8222, 272.8903 },
				{ 2588.0132, 2799.4094, 12.8203, 92.4087 },
				{ 2594.9827, 2788.6501, 12.8203, 138.4691 },
				{ 2592.2373, 2738.7629, 12.9844, 93.0354 },
				{ 2580.7913, 2753.3501, 12.8203, 111.9805 },
				{ 2551.9824, 2710.2253, 12.8203, 355.4194 },
				{ 2501.4497, 2725.7554, 12.9844, 281.3036 },
				{ 2538.5042, 2760.2688, 20.3141, 272.3854 },
				{ 2670.9207, 2818.5720, 38.3222, 179.7829 },
				{ 2626.4617, 2831.4512, 23.3222, 337.6810 },
				{ 0.0f, 0.0f, 0.0f, 0.0f }
		}),
		ArenaObjects({
			{ 4186, 2071.57, 999.02, 1517.97, 0.00, 0.00, 0.00 },
			{ 4186, 2071.54, 1060.48, 1517.94, 0.00, 0.00, 540.00 },
			{ 2780, 2593.9058, 2730.4553, 22.8222, 0.0000, 0.0000, 0.0000 },
			{ 2780, 2591.9272, 2737.1333, 25.4188, 0.0000, 0.0000, 0.0000 },
			{ 2780, 2594.1128, 2741.5513, 22.8222, 0.0000, 0.0000, 0.0000 },
			{ 2780, 2594.1609, 2747.1543, 22.8222, 0.0000, 0.0000, 0.0000 },
			{ 2780, 2594.2297, 2752.2192, 22.8222, 0.0000, 0.0000, 0.0000 },
			{ 2780, 2594.1113, 2757.8596, 22.8222, 0.0000, 0.0000, 0.0000 },
			{ 2780, 2594.1443, 2763.7427, 22.8222, 0.0000, 0.0000, 0.0000 },
			{ 2780, 2594.1951, 2770.4026, 22.8222, 0.0000, 0.0000, 0.0000 },
			{ 2780, 2594.0869, 2776.0537, 22.8222, 0.0000, 0.0000, 0.0000 },
			{ 2780, 2594.2515, 2781.9097, 22.8222, 0.0000, 0.0000, 0.0000 },
			{ 2780, 2593.936, 2724.8799, 22.8222, 0.0000, 0.0000, 0.0000 },
			{ 3279, 2543.4893, 2811.1978, 26.8959, 0.0000, 0.0000, 90.0000 },
			{ 3279, 2604.769, 2808.6655, 26.8959, 0.0000, 0.0000, 0.0000 },
			{ 3279, 2507.4285, 2715.843, 9.8959, 0.0000, 0.0000, 90.0000 },
			{ 3279, 2504.9041, 2768.9839, 9.8959, 0.0000, 0.0000, 0.0000 },
			{ 3279, 2626.4038, 2728.1233, 35.6143, 0.0000, 0.0000, 180.0000 },
			{ 3279, 2640.7568, 2815.9836, 37.3978, 0.0000, 0.0000, 270.0000 },
			{ 3279, 2700.803, 2815.7751, 37.3978, 0.0000, 0.0000, 270.0000 },
			{ 3279, 2636.0303, 2776.3311, 24.8978, 0.0000, 0.0000, 270.0000 },
			{ 3279, 2597.3052, 2789.583, 15.6478, 0.0000, 0.0000, 90.0000 },
			{ 3279, 2578.1672, 2845.2424, 26.8959, 0.0000, 0.0000, 270.0000 },
			{ 3279, 2657.1401, 2849.1011, 9.8959, 0.0000, 0.0000, 90.0000 },
			{ 3279, 2742.106, 2849.9358, 9.8959, 0.0000, 0.0000, 225.0000 },
			{ 3279, 2711.8909, 2713.0359, 9.8959, 0.0000, 0.0000, 90.0000 },
			{ 3279, 2685.9534, 2747.1521, 19.3978, 0.0000, 0.0000, 0.0005 },
			{ 3279, 2609.8828, 2713.9519, 35.6143, 0.0000, 0.0000, 0.0005 },
			{ 1225, 2625.113, 2726.4038, 52.0981, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2626.1067, 2726.4116, 52.0981, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2608.9219, 2712.1653, 52.0981, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2609.9348, 2712.167, 52.0981, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2606.7231, 2731.5127, 35.9444, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2606.708, 2733.2161, 35.9444, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2606.7021, 2732.3701, 35.9444, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2606.7183, 2730.655, 35.9444, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2606.7393, 2729.7888, 35.9444, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2630.1523, 2709.6396, 35.9444, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2629.1516, 2709.936, 35.9444, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2629.9307, 2710.6143, 35.9444, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2606.679, 2732.7561, 37.0291, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2606.6814, 2731.8892, 37.0292, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2606.6833, 2731.0107, 37.024, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2606.6838, 2730.1428, 37.0195, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2606.6716, 2730.5947, 38.122, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2606.6921, 2731.4868, 38.1077, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2606.6638, 2732.3503, 38.0911, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2634.7102, 2778.1255, 41.3817, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2633.9741, 2777.4229, 41.3817, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2642.564, 2814.6455, 53.8817, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2641.8472, 2813.9939, 53.8817, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2602.9331, 2807.4019, 43.3798, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2603.7529, 2806.8169, 43.3798, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2595.5876, 2788.5969, 32.1317, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2596.2461, 2787.9548, 32.1317, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2699.5061, 2817.6804, 53.8817, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2698.9993, 2816.8594, 53.8817, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2684.9001, 2748.988, 35.8817, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2684.123, 2748.4429, 35.8817, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2538.6414, 2827.1135, 10.2261, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2538.5056, 2828.2197, 10.2261, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2538.428, 2829.3232, 10.2261, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2596.752, 2818.5886, 10.2261, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2589.7805, 2814.4846, 10.2261, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2605.6692, 2816.6001, 10.2261, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2602.0222, 2840.2554, 10.2261, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2599.6455, 2837.0713, 10.2261, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2591.9792, 2839.011, 10.2261, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2553.1521, 2836.1299, 10.2261, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2557.3462, 2836.1235, 10.2261, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2563.1272, 2838.7649, 10.2261, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2567.2341, 2820.7898, 10.2261, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2564.2537, 2820.9785, 10.2261, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2564.8633, 2807.4487, 10.2261, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2568.7449, 2809.1873, 10.2261, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2574.095, 2808.9119, 10.2261, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2562.1343, 2836.0708, 10.2261, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2549.7292, 2821.5769, 10.2261, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2549.7256, 2820.6487, 10.2261, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2549.3301, 2826.363, 10.2261, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2571.0759, 2841.6521, 13.6617, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2570.1997, 2841.3293, 13.6617, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2602.7986, 2812.6545, 13.6617, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2601.7383, 2812.595, 13.6617, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2597.8384, 2810.3962, 13.6617, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2598.2607, 2811.5884, 13.6617, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2542.7175, 2814.3569, 13.6617, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2548.0344, 2814.085, 13.6617, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2544.2583, 2821.7451, 13.6617, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2542.3831, 2826.9915, 13.6617, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2547.8303, 2830.9202, 13.6617, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2548.1719, 2842.9854, 13.6617, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2543.8167, 2842.4585, 13.6617, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2546.1113, 2835.7544, 10.2261, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2626.105, 2831.9253, 10.2261, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2627.335, 2842.0889, 10.2261, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2625.2476, 2842.0828, 10.2261, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2624.2383, 2842.0967, 10.2261, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2626.2366, 2842.002, 10.2261, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2621.1741, 2846.4072, 17.0954, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2619.8811, 2846.6567, 13.6617, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2625.1206, 2853.5955, 13.6617, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2627.3147, 2829.1084, 20.728, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2630.8535, 2835.4636, 23.478, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2617.6394, 2824.8784, 22.8276, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2611.8997, 2788.2136, 15.978, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2611.9055, 2787.3418, 15.978, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2611.7319, 2797.0332, 15.978, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2607.2986, 2802.4063, 22.8276, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2613.0527, 2797.5645, 10.2261, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2626.4871, 2806.5168, 10.2261, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2626.897, 2805.5627, 10.2261, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2666.1836, 2795.7344, 13.6617, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2664.9431, 2794.3047, 13.6617, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2645.708, 2783.2483, 10.2261, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2647.2979, 2729.8645, 10.2261, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2648.2808, 2729.8911, 10.2261, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2664.6636, 2744.3542, 13.6617, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2664.3975, 2764.0701, 13.6617, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2682.5044, 2750.8418, 19.728, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2655.9883, 2696.7537, 18.728, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2637.1841, 2773.2068, 25.228, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2636.2559, 2773.1895, 25.228, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2653.8008, 2765.4236, 18.728, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2645.6187, 2768.2129, 23.228, 0.0000, 0.0000, 0.0000 },
			{ 1225, 2593.0481, 2802.5217, 10.2261, 0.0000, 0.0000, 0.0000 },
			{ 12839, 2598.5032, 2798.6841, 12.8146, 0.0000, 0.0000, 270.0000 },
			{ 14410, 2614.6689, 2784.5466, 12.3616, 0.0000, 0.0000, 90.0000 },
			{ 16082, 2538.0322, 2759.2849, 13.6678, 0.0000, 0.0000, 0.0000 },
			{ 3399, 2602.1228, 2852.0596, 24.567, 0.0000, 0.0000, 0.0000 },
			{ 3399, 2590.4597, 2852.0376, 19.8687, 0.0000, 0.0000, 0.0000 },
			{ 3399, 2578.8794, 2852.0127, 15.1865, 0.0000, 0.0000, 0.0000 },
			{ 3399, 2570.2659, 2851.9966, 12.1575, 0.0000, 0.0000, 0.0000 },
			{ 10009, 2513.2371, 2792.927, 14.3811, 0.0000, 0.0000, 180.0000 },
			{ 3271, 2722.6677, 2750.0732, 10.1145, 0.0000, 0.0000, 191.2500 },
			{ 3794, 2624.6108, 2710.8909, 36.138, 0.0000, 0.0000, 315.0000 },
			{ 3794, 2622.3171, 2710.9834, 36.138, 0.0000, 0.0000, 315.0000 },
			{ 3794, 2620.0789, 2710.936, 36.138, 0.0000, 0.0000, 315.0000 },
			{ 12911, 2529.4709, 2712.3132, 9.8167, 0.0000, 0.0000, 101.2500 },
			{ 12912, 2585.3994, 2763.8953, 21.3167, 0.0000, 0.0000, 0.0000 },
			{ 16641, 2568.5298, 2779.6104, 11.5752, 0.0000, 0.0000, 270.0000 },
			{ 16641, 2549.3142, 2777.3811, 11.5752, 0.0000, 0.0000, 0.0000 },
			{ 16641, 2548.2729, 2836.7761, 28.5752, 0.0000, 0.0000, 270.0000 },
			{ 16641, 2561.8918, 2813.147, 28.5752, 0.0000, 0.0000, 337.5000 },
			{ 16641, 2579.2271, 2837.0432, 11.5752, 0.0000, 0.0000, 0.0000 },
			{ 16641, 2584.479, 2807.1101, 11.5752, 0.0000, 0.0000, 180.0000 },
			{ 2899, 2539.5381, 2823.3198, 9.9373, 0.0000, 0.0000, 0.0000 },
			{ 2899, 2616.2173, 2830.936, 9.9373, 0.0000, 0.0000, 0.0000 },
			{ 2917, 2569.9431, 2832.7002, 18.3293, 0.0000, 0.0000, 0.0000 },
			{ 2918, 2603.1367, 2765.95, 25.6864, 0.0000, 0.0000, 0.0000 },
			{ 2918, 2599.8147, 2773.7717, 25.6113, 0.0000, 0.0000, 0.0000 },
			{ 2921, 2616.2327, 2803.2344, 26.5175, 0.0000, 0.0000, 90.0000 },
			{ 2923, 2541.9224, 2766.7815, 9.8203, 0.0000, 0.0000, 90.0000 },
			{ 2932, 2553.4648, 2759.5725, 11.2722, 0.0000, 0.0000, 90.0000 },
			{ 2934, 2559.8958, 2757.1333, 11.2722, 0.0000, 0.0000, 0.0000 },
			{ 2935, 2553.4946, 2755.1201, 11.2722, 0.0000, 0.0000, 90.0000 },
			{ 2936, 2539.7683, 2766.7952, 10.2706, 0.0000, 0.0000, 0.0000 },
			{ 2976, 2537.7664, 2761.7268, 17.3081, 0.0000, 0.0000, 0.0000 },
			{ 2985, 2546.584, 2757.197, 17.3456, 0.0000, 0.0000, 0.0000 },
			{ 3073, 2528.0129, 2737.3967, 11.4253, 0.0000, 0.0000, 270.0000 },
			{ 3099, 2539.2458, 2740.0779, 9.958, 0.0000, 0.0000, 90.0000 },
			{ 3110, 2506.1089, 2746.3472, 7.5651, 0.0000, 0.0000, 112.5001 },
			{ 967, 2505.4912, 2781.2104, 9.8203, 0.0000, 0.0000, 0.0000 },
			{ 3866, 2569.1055, 2743.9255, 17.4839, 0.0000, 0.0000, 180.0000 },
			{ 3887, 2739.6714, 2808.8777, 18.6888, 0.0000, 0.0000, 180.0000 },
			{ 11428, 2587.6306, 2811.0496, 32.7238, 0.0000, 0.0000, 90.0000 },
			{ 11442, 2609.3589, 2827.4543, 26.9941, 0.0000, 0.0000, 90.0000 },
			{ 11443, 2610.9124, 2838.4185, 26.9921, 0.0000, 0.0000, 90.0000 },
			{ 11444, 2584.8176, 2845.1479, 26.8133, 0.0000, 0.0000, 0.0000 },
			{ 16770, 2510.1587, 2810.3625, 11.4109, 0.0000, 0.0000, -0.0000 },
			{ 5126, 2522.5273, 2840.7048, 24.9447, 0.0000, 0.0000, 270.0000 },
			{ 5262, 2521.1367, 2748.7246, 12.778, 0.0000, 0.0000, 0.0000 },
			{ 5262, 2634.9763, 2719.5049, 27.7299, 0.0000, 0.0000, 90.0001 },
			{ 5269, 2629.1875, 2716.4001, 37.8477, 0.0000, 0.0000, 0.0000 },
			{ 7040, 2740.4573, 2727.283, 13.2481, 0.0000, 0.0000, 90.0000 },
			{ 7025, 2735.5994, 2712.186, 13.2481, 0.0000, 0.0000, 0.0000 },
			{ 7040, 2725.7229, 2724.8035, 13.2481, 0.0000, 0.0000, 0.0000 },
			{ 8885, 2599.7747, 2726.5137, 26.25, 0.0000, 0.0000, 0.0000 },
			{ 8885, 2637.1379, 2787.5364, 13.2481, 0.0000, 0.0000, 0.0000 },
			{ 12913, 2616.8154, 2792.0955, 12.4456, 0.0000, 0.0000, 0.0000 },
			{ 13489, 2556.2209, 2845.5356, 29.4379, 0.0000, 0.0000, 90.0000 },
			{ 16601, 2553.8213, 2746.4104, 14.5594, 0.0000, 0.0000, 0.0000 },
			{ 16601, 2549.4177, 2746.3564, 14.5594, 0.0000, 0.0000, 0.0000 },
			{ 16601, 2545.2078, 2746.292, 14.5594, 0.0000, 0.0000, 0.0000 },
			{ 1682, 2508.896, 2728.0029, 16.2672, 0.0000, 0.0000, 292.5000 },
			{ 1682, 2567.2083, 2832.0002, 33.2745, 0.0000, 0.0000, 202.5000 },
			{ 1682, 2575.4951, 2712.8125, 33.6495, 0.0000, 0.0000, 22.5000 },
			{ 10757, 2512.2788, 2759.3745, 36.786, 0.0000, 0.0000, 123.7499 },
			{ 10763, 2534.425, 2683.2627, 42.5807, 0.0000, 0.0000, 0.0000 },
			{ 6056, 2524.6218, 2761.717, 15.0928, 0.0000, 0.0000, 270.0000 },
			{ 7415, 2530.5854, 2718.9248, 19.6634, 0.0000, 0.0000, 303.7500 },
			{ 10281, 2546.7209, 2802.2344, 20.0422, 0.0000, 0.0000, 0.0000 },
			{ 10838, 2499.5542, 2811.021, 29.6996, 0.0000, 0.0000, 0.0000 },
			{ 11395, 2528.6577, 2773.0327, 17.8907, 0.0000, 0.0000, 180.0000 },
			{ 14467, 2607.7878, 2718.7385, 38.2724, 0.0000, 0.0000, 146.2500 },
			{ 16480, 2679.0012, 2748.3438, 20.8711, 0.0000, 0.0000, 225.0000 },
			{ 3399, 2696.2808, 2753.9812, 12.1644, 0.0000, 0.0000, 270.0000 },
			{ 3399, 2696.2783, 2744.0457, 15.8394, 0.0000, 0.0000, 270.0000 },
			{ 14414, 2623.1431, 2818.4766, 32.0547, 0.0000, 0.0000, 90.0000 },
			{ 14414, 2609.4136, 2818.4785, 25.7048, 0.0000, 0.0000, 90.0000 },
			{ 14414, 2611.5759, 2787.0425, 25.9567, 0.0000, 0.0000, 180.0000 },
			{ 14414, 2611.5476, 2800.9104, 25.9364, 0.0000, 0.0000, 0.0000 }
				})
				{}

		bool OnGameModeInit() override
		{
			MakeAllDynamicObjects(ArenaObjects, CurrentGameVW);
			return true;
		}

		void Timer(int timerid, ZabawyTimerVector& vector)
		{
			int playerid = TimerDataGet<int>(vector, 0);
			if (Player[playerid].CurrentGameID == this)
			{
				safeGivePlayerWeapon(playerid, 38, 999999);
			}
		}

		void PutPlayerIntoGame(int playerid)
		{
			fixSetPlayerHealth(playerid, 100);
			fixSetPlayerArmour(playerid, 100);

			auto i = ArenaSpawns[Functions::RandomGenerator->Random(0, ArenaSpawns.size() - 1)];
			TeleportPlayer(playerid, i.x, i.y, i.z, i.a, false, 0, CurrentGameVW, "", 0, 0, 0.0, 0.0, true);

			CreateTimer<Arena>(333, false, &Arena::Timer, playerid);
			
		}

		bool OnKeepInGameSpawn(int playerid) override
		{
			PutPlayerIntoGame(playerid);
			return true;
		}

		bool OnGameCommandExecute(int playerid, std::string params) override
		{
			AddPlayer(playerid, true, 0xFFFFFF00);
			PutPlayerIntoGame(playerid);
			return true;
		}

		bool OnPlayerDeath(int playerid, int killerid, int reason) override
		{
			GivePlayerAchievement(playerid, EAM_ZabawyMinigunDeaths, 1);
			if (killerid != INVALID_PLAYER_ID)
			{
				GivePlayerAchievement(killerid, EAM_ZabawyMinigunKills, 1);
			}
			return true;
		}

		bool PlayerRequestGameExit(int playerid, int reason) override
		{
			switch (reason)
			{
			case PlayerRequestExitTypeDisconnect:
				PlayerExitGame(playerid);
				break;
			case PlayerRequestExitTypeExit:
				PlayerExitGame(playerid);
				break;
			}
			return true;
		}
	} _Arena;
}