Guide d'Utilisation du Simulateur de Gestion Simplifiée de Fichiers (SGF
               
Bienvenue dans le simulateur de gestion simplifiée de fichiers (SGF). Ce document vous guide pour comprendre et utiliser les fonctionnalités principales du programme.

Prérequis
Un compilateur C (comme GCC ou Clang) pour compiler le code.
Un terminal ou une interface IDE pour exécuter le programme.
Compilation et Exécution
Compilez le code source :
gcc -o simulateur simulateur.c
Exécutez le programme :
./simulateur
Fonctionnalités Principales
Le programme présente un menu principal permettant d’interagir avec les différentes fonctionnalités. Voici un résumé des fonctionnalités principales, organisées par rôle :
1: Core File System Operations
Enregistrement et gestion des blocs :
Initialisation et manipulation des blocs de mémoire secondaire.
Gestion des métadonnées de base.
Création des fichiers :
Supporte les modes contiguë et chaîné.
Modification des blocs après suppression, insertion ou création.
2: Advanced File Management
Renommer un fichier :
Modifier le nom d’un fichier existant.
Suppression logique et physique :
Gestion des fichiers supprimés de manière temporaire ou définitive.
Recherche et insertion d’enregistrements :
Localiser et ajouter des entrées dans les fichiers.
Chargement des fichiers en mémoire secondaire :
Transférer des fichiers dans les blocs pour les opérations.
Compactage :
Proposer un compactage automatique en cas d’espace insuffisant.
Fonction de réduction des fragments.
3: Disk Initialization and Maintenance
Initialisation du disque :
Configuration de la structure initiale et des métadonnées globales.
Vidage de la mémoire secondaire :
Suppression complète des blocs pour réinitialiser le disque.
Défragmentation des blocs :
Regroupement des blocs utilisés pour optimiser l’espace.
Défragmentation d’un fichier :
Réorganisation des blocs associés à un fichier précis.
Mise à jour des métadonnées :
Ajustement des informations liées aux blocs (libres ou utilisés) et mise à jour des liens chaînés après chaque opération.
Gestion des blocs libres :
 Vérification avant chaque opération et gestion des erreurs en cas de mémoire pleine.
Utilisation des Fonctionnalités
Le programme présente un menu interactif pour accéder à toutes les fonctionnalités précédentes.
Chaque fonctionnalité peut être sélectionnée via le menu principal en suivant les instructions affichées. Voici une description des principales options :
Afficher l'état du disque
Affiche l’état actuel du disque, incluant :
Le nombre total de blocs.
Le nombre de blocs libres et utilisés.
L’occupation de chaque bloc (« Libre » ou « Utilisé »).
Défragmenter le disque
Rassemble tous les blocs utilisés sur le disque pour les rendre contigus.
Efface les anciens emplacements pour optimiser l’espace.
Défragmenter un fichier
Réorganise les blocs associés à un fichier spécifique pour les rendre contigus.
Met à jour les chaînages et les métadonnées associées au fichier.
Lorsqu'invité, entrez le nom du fichier à défragmenter (par exemple, file1.txt).
Renommer un fichier
Permet de modifier le nom d’un fichier existant.
Entrez le nom actuel du fichier, suivi du nouveau nom.
Suppression d’un fichier
Supprime un fichier soit logiquement, soit physiquement.
Spécifiez le fichier à supprimer lorsque cela est demandé.

Compactage des blocs
Propose de regrouper les blocs non utilisés pour optimiser l’espace.
Cette option peut être activée automatiquement ou via le menu principal.
Quitter
Termine le programme proprement.
Exemple de Scénario d’Utilisation
Lancez le programme.
Utilisez l’option pour afficher l’état initial du disque.
Ajoutez ou simulez des fichiers et des blocs (intégrés dans le code).
Lancez la défragmentation d’un fichier précis.
Effectuez une défragmentation globale du disque.
Affichez à nouveau l’état du disque pour observer les changements.
Quittez le programme.
Débogage et Problèmes Courants
Erreur : « Fichier introuvable » :
Assurez-vous que le nom du fichier entré correspond exactement à celui défini dans le simulateur.
Le programme ne compile pas :
Vérifiez que le fichier source est complet et que GCC est correctement installé.
Problèmes de chaînage :
Vérifiez les métadonnées et les liens chaînés dans les blocs.

Limitations Connues
Le simulateur ne gère pas encore :
Les fichiers avec des tailles dynamiques.
Les opérations parallèles sur les fichiers.

Crédits
Ce simulateur a été développé dans le cadre d’un projet collaboratif par :
Manel fatma Ameziane : Opérations de base sur les fichiers.
Bouasria Chaima : Gestion avancée des fichiers.
Rihane Ghazi : Initialisation et maintenance du disque.
Conclusion
Ce simulateur illustre les principes de base d'un système de gestion de fichiers, offrant des fonctionnalités essentielles comme la défragmentation,
la gestion des blocs, et la maintenance des fichiers. Merci d’avoir utilisé cet outil, et n’hésitez pas à l’améliorer !
